#include "buffer.h++"

namespace nutsloop {

nuts_buffer_t &buffer::get() {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    BUFFER << "buffer::get() called ⇣" << '\n'
           << "is read_ " << std::boolalpha
           << std::format("[ {} ]", get_read_()) << '\n'
           << "is allocated_ " << std::boolalpha
           << std::format("[ {} ]", get_allocated_()) << '\n'
           << "is empty " << std::boolalpha
           << std::format("[ {} ]", nuts_buffer_.empty()) << '\n';
  }
#endif

  return nuts_buffer_;
}

nuts_buffer_unlined_t &buffer::get(const size_t &line) {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    BUFFER << std::format("buffer::get(line[{}]) called ⇣", line) << '\n'
           << "is read_ " << std::boolalpha
           << std::format("[ {} ]", get_read_()) << '\n'
           << "is allocated_ " << std::boolalpha
           << std::format("[ {} ]", get_allocated_()) << '\n'
           << "is empty " << std::boolalpha
           << std::format("[ {} ]", nuts_buffer_.empty()) << '\n';
  }
#endif

  return nuts_buffer_[line];
}

nuts_byte_t &buffer::get(const size_t &line, const size_t &col) {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);

    BUFFER << std::format("buffer::get(line[{}], col[{}]) called ⇣", line, col)
           << '\n'
           << "is read_ " << std::boolalpha
           << std::format("[ {} ]", get_read_()) << '\n'
           << "is allocated_ " << std::boolalpha
           << std::format("[ {} ]", get_allocated_()) << '\n'
           << "is empty " << std::boolalpha
           << std::format("[ {} ]", nuts_buffer_.empty()) << '\n';
  }
#endif

  return nuts_buffer_[line][col];
}

} // namespace nutsloop
