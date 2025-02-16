#include "buffer.h++"

namespace nutsloop {

/*
 * FIXME: all the stream methods aren't thread safe.
 *        necessary to implement a registry that handles different buffer stream
 *        now if there are more than one instance of buffer and "all" of the instances use ::stream
 *        the global variable are updated and set for all the instances of buffer.
 */

/*
 * TODO: DEBUG LOG
 * TODO: handle exceptions
 * TODO: overload to return also the null byte
*/
nuts_buffer_stream_t buffer::stream() {

  if (stream_active_.load() == false) {
    stream_active_.exchange(true);
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    // If we've reached the end of the buffer, return nullopt to end the stream.
    if (stream_line_.load() >= size()) {

      stream_line_.store(0);          // Reset line index
      stream_col_.store(0);           // Reset column index
      stream_byte_.store(byte(0x00)); // Reset byte

      return std::nullopt; // End of stream
    }

    // If we've finished the current line, return '\n'
    if (stream_col_.load() == stream_line_strip_null_byte_()) {
      return nuts_buffer_stream_t({
          {stream_line_.fetch_add(1), stream_col_.exchange(0)},
          byte(0x0A) // Return newline
      });
    }

    return nuts_buffer_stream_t({{stream_line_.load(), stream_col_.load()},
                                 get(stream_line_.load(), stream_col_.fetch_add(1))});
  }
}

std::size_t buffer::stream_line_strip_null_byte_() const {
  // the buffer always has a null byte at the end of each line.
  // for line, it means vector<byte> `nuts_buffer_unlined_t` check the file
  // types.h++ for the definition
  // every nuts_buffer_unlined_t doesn't have a new line character.
  return size(stream_line_.load()) - 1;
}

// TODO: handle exceptions
std::optional<nuts_byte_t> buffer::stream(const std::size_t search_at_line,
                                          const std::size_t from_col_n,
                                          const nuts_byte_t until_it_finds) {

  if (stream_active_.load() == false) {
    stream_active_.exchange(true);
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);

    // select the line to search
    stream_line_.store(search_at_line);    // set the line index
    stream_col_.store(from_col_n);         // set the column index
    stream_byte_.store(nuts_byte_t{0x00}); // Reset byte

    if (get(search_at_line, from_col_n) != until_it_finds) {
      stream_col_.fetch_add(1);                            // Move to the next byte
      stream_byte_.store(get(search_at_line, from_col_n)); // Return next byte
      return stream_byte_;
    }

    return std::nullopt;
  }
}

// TODO: must be adapted to modifiers
// TODO: handle exceptions
nuts_buffer_stream_t buffer::stream(const size_t line) {

  if (stream_active_.load() == false) {
    stream_active_.exchange(true);
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);

    // check if the line is valid
    if (line >= size()) {
      return std::nullopt; // No such line
    }

    stream_line_.store(line); // set the line index

    // End of line
    if (stream_col_.load() == stream_line_strip_null_byte_()) {
      stream_col_.store(0);           // Reset column index
      stream_line_.store(0);          // Reset line index
      stream_byte_.store(byte(0x00)); // Reset byte
      return std::nullopt;            // Signal end of the line
    }

    // Return the next byte in the line
    return nuts_buffer_stream_t({{stream_line_.load(), stream_col_.load()},
                                 get(stream_line_.load(), stream_col_.fetch_add(1))});
  }
}

// TODO: handle exceeding sizes
std::size_t buffer::set_stream_line(const std::size_t line_n) {

  if (!stream_active_.load()) {
    // TODO: explain things
    throw std::runtime_error("buffer::set_stream_line(): stream is not active");
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    stream_col_.store(0);
    if (line_n > size()) {
      throw std::runtime_error("buffer::set_stream_line(): line index exceeds buffer size");
    }
    return stream_line_.exchange(line_n);
  }
}

// TODO: handle exceeding sizes
std::size_t buffer::set_stream_col(const std::size_t col_n) {

  if (!stream_active_.load()) {
    // TODO: explain things
    throw std::runtime_error("buffer::set_stream_col(): stream is not active");
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    if (col_n > size(stream_line_)) {
      throw std::runtime_error("buffer::set_stream_col(): column index exceeds buffer size");
    }
    return stream_col_.exchange(col_n);
  }
}

std::size_t buffer::end_stream() {

  if (!stream_active_.load()) {
    // TODO: explain things
    throw std::runtime_error("buffer::end_stream(): stream is not active");
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    return stream_line_.exchange(size());
  }
}

} // namespace nutsloop
