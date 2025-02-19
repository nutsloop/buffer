#include "buffer.h++"

namespace nutsloop {

/*
 * FIXME: all the stream methods aren't thread safe.
 *        necessary to implement a registry that handles different buffer stream
 *        now if there are more than one instance of buffer and "all" of the instances use ::stream
 *        the global variable are updated and set for all the instances of buffer.
 */
/*
 * the buffer always has a null byte at the end of each line.
 * for line, it means vector<byte> `nuts_buffer_unlined_t` check the file
 * types.h++ for the definition
 * every nuts_buffer_unlined_t doesn't have a new line character.
 */
/*
 * TODO: implement a next method.
 * TODO: DEBUG LOG.
 * TODO: handle exceptions.
 * TODO: overload to return also the null byte.
 */
nuts_buffer_stream_t buffer::stream::next(const bool strip_null_byte) {

  if (!active_.load()) {
    active_.store(true);
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    // If we've reached the end of the buffer, return nullopt to end the stream.
    if (line_.load() >= buffer_->size()) {

      line_.store(0);          // Reset line index
      column_.store(0);        // Reset column index
      byte_.store(buffer_->byte(0x00)); // Reset byte

      return std::nullopt; // End of stream
    }

    // If we've finished the current line, return '\n'
    if (column_.load() == buffer_->size(line_.load()) - (strip_null_byte ? 1 : 0)) {
      return nuts_buffer_stream_t({
          {line_.fetch_add(1), column_.exchange(0)},
        buffer_->byte(0x0A) // Return newline
      });
    }

    return nuts_buffer_stream_t({{line_.load(), column_.load()},
                                 buffer_->get(line_.load(), column_.fetch_add(1))});
  }
}

// TODO: handle exceptions
// TODO: handle end of the line or other until_it_finds
// this method start a stream at a given line at a given column until it finds the given byte.
// it increments the column index until it finds the given byte.
// it returns all the bytes until it finds the given byte.
std::optional<nuts_byte_t> buffer::stream::next(const std::size_t search_at_line,
                                          const std::size_t from_col_n,
                                          const nuts_byte_t until_it_finds) {

  if (!active_.load()) {
    active_.store(true);
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);

    // select the line to search
    line_.store(search_at_line);    // set the line index
    column_.store(from_col_n);      // set the column index
    byte_.store(nuts_byte_t{0x00}); // Reset byte

    if (buffer_->get(search_at_line, from_col_n) != until_it_finds) {
      column_.fetch_add(1);                         // Move to the next byte
      byte_.store(buffer_->get(search_at_line, from_col_n)); // Return next byte
      return byte_.load();
    }

    return std::nullopt;
  }
}

// TODO: must be adapted to modifiers
// TODO: handle exceptions
nuts_buffer_stream_t buffer::stream::next(const size_t line, const bool strip_null_byte) {

  if (!active_.load()) {
    active_.store(true);
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);

    // check if the line is valid
    if (line >= buffer_->size()) {
      return std::nullopt; // No such line
    }

    line_.store(line); // set the line index

    // End of line
    if (column_.load() == buffer_->size(line_.load()) - (strip_null_byte ? 1 : 0)) {
      column_.store(0);        // Reset column index
      line_.store(0);          // Reset line index
      byte_.store(buffer_->byte(0x00)); // Reset byte
      return std::nullopt;            // Signal end of the line
    }

    // Return the next byte in the line
    return nuts_buffer_stream_t({{line_.load(), column_.load()},
                                 buffer_->get(line_.load(), column_.fetch_add(1))});
  }
}

// TODO: handle exceeding sizes
std::size_t buffer::stream::move_at_line(const std::size_t line_n) {

  if (!active_.load()) {
    // TODO: explain things
    throw std::runtime_error("buffer::stream_::move_at_line(): stream is not active");
  }

  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    column_.store(0);
    if (line_n > buffer_->size()) {
      throw std::runtime_error("buffer::stream_::move_at_line(): line index exceeds buffer size");
    }
    return line_.exchange(line_n);
  }
}

// TODO: handle exceeding sizes
std::size_t buffer::stream::move_at_column(const std::size_t col_n) {

  if (!active_.load()) {
    // TODO: explain things
    throw std::runtime_error("buffer::stream_::move_at_column(): stream is not active");
  }

  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    if (col_n > buffer_->size(line_.load())) {
      throw std::runtime_error("buffer::stream_::move_at_column(): column index exceeds buffer size");
    }

    return column_.exchange(col_n);
  }
}

std::size_t buffer::stream::ends() {

  if (!active_.load()) {
    // TODO: explain things
    throw std::runtime_error("buffer::stream_::ends(): stream is not active");
  }

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    return line_.exchange(buffer_->size());
  }
}

}
