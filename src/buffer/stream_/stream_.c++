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
nuts_buffer_stream_t buffer::stream_::next(const bool strip_null_byte) {

  {
    // MARK: (buffer) SHARED_LOCK
    std::shared_lock lock(mtx_);

    // If we've reached the end of the buffer, return nullopt to end the stream.
    if (line_.load() >= buffer_->size()) {

      line_.store(0);     // Reset line index
      column_.store(0);   // Reset column index
      byte_ = null_byte_; // Reset byte

      return std::nullopt; // End of stream
    }

    // If we've finished the current line, return '\n'
    if (column_.load() == buffer_->size(line_.load()) - (strip_null_byte ? 1 : 0)) {

      if (buffer_->get_from_string_()) {

        line_.store(0);     // Reset line index
        column_.store(0);   // Reset column index
        byte_ = null_byte_; // Reset byte

        return std::nullopt;
      }

      return nuts_buffer_stream_t({
          {line_.fetch_add(1), column_.exchange(0)},
          newline_byte_ // Return newline
      });
    }

    return nuts_buffer_stream_t(
        {{line_.load(), column_.load()}, buffer_->get(line_.load(), column_.fetch_add(1))});
  }
}

// TODO: handle exceptions
// TODO: handle end of the line or other until_it_finds
// this method start a stream at a given line at a given column until it finds the given byte.
// it increments the column index until it finds the given byte.
// it returns all the bytes until it finds the given byte.
std::optional<nuts_byte_t> buffer::stream_::next(const std::size_t search_at_line,
                                                 const std::size_t from_col_n,
                                                 const nuts_byte_t &until_it_finds) {

  {
    // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);

    // select the line to search
    line_.store(search_at_line); // set the line index
    column_.store(from_col_n);   // set the column index
    byte_ = null_byte_;          // Reset byte

    if (buffer_->get(search_at_line, from_col_n) != until_it_finds) {
      column_.fetch_add(1);                               // Move to the next byte
      byte_ = {buffer_->get(search_at_line, from_col_n)}; // Return next byte
      return byte_;
    }

    return std::nullopt;
  }
}

// TODO: must be adapted to modifiers
// TODO: handle exceptions
nuts_buffer_stream_t buffer::stream_::next(const size_t line, const bool strip_null_byte) {

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
      column_.store(0);    // Reset column index
      line_.store(0);      // Reset line index
      byte_ = null_byte_;  // Reset byte
      return std::nullopt; // Signal end of the line
    }

    // Return the next byte in the line
    return nuts_buffer_stream_t(
        {{line_.load(), column_.load()}, buffer_->get(line_.load(), column_.fetch_add(1))});
  }
}

// TODO: handle exceeding sizes
std::size_t buffer::stream_::move_at_line(const std::size_t line_n) {

  { // MARK: (buffer) SHARED_LOCK
    std::shared_lock lock(mtx_);
    column_.store(0);
    if (line_n > buffer_->size()) {
      throw std::runtime_error("buffer::stream_::move_at_line(): line index exceeds buffer size");
    }
    return line_.exchange(line_n);
  }
}

// TODO: handle exceeding sizes
std::size_t buffer::stream_::move_at_column(const std::size_t col_n) {

  { // MARK: (buffer) SHARED_LOCK
    std::shared_lock lock(mtx_);
    if (col_n > buffer_->size(line_.load())) {
      throw std::runtime_error(
          "buffer::stream_::move_at_column(): column index exceeds buffer size");
    }

    return column_.exchange(col_n);
  }
}

std::size_t buffer::stream_::ends() {

  {
    // MARK: (buffer) SHARED_LOCK
    std::shared_lock lock(mtx_);
    return line_.exchange(buffer_->size());
  }
}
void buffer::stream_::operator-(
    [[maybe_unused]] std::optional<std::tuple<std::size_t>> coordinates) {
  // TODO: implement the coordinates tuple to remove the byte at the given location
  {
    // MARK: (buffer) SHARED_LOCK
    std::shared_lock lock(mtx_);
    nuts_buffer_unlined_t &buf = buffer_->get(line_.load());
    buf.erase(buf.begin() + diff_(column_));
  }
}

void buffer::stream_::operator+(const nuts_byte_t &byte) {

  {
    // MARK: (buffer) SHARED_LOCK
    std::shared_lock lock(mtx_);
    byte_ = byte;
    nuts_buffer_unlined_t &buf = buffer_->get(line_.load());
    buf.insert(buf.begin() + diff_(column_), byte_);
  }
}

buffer::stream_ &buffer::stream_::operator=(const nuts_byte_t &byte) {

  {
    // MARK: (buffer) SHARED_LOCK
    std::shared_lock lock(mtx_);
    byte_ = byte;
    nuts_buffer_unlined_t &buf = buffer_->get(line_.load());
    buf[column_.load()] = byte_;
  }
  return *this;
}

nuts_diff_t buffer::stream_::diff_(const std::atomic<std::size_t> &index) {
  return static_cast<nuts_diff_t>(index.load());
}

} // namespace nutsloop
