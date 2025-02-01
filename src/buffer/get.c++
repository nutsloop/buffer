#include "buffer.h++"

namespace nutsloop {

nuts_buffer_t& buffer::get() {

  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock( mtx_ );
    if (DEBUG_BUFFER_CONST) {
      BUFFER << "buffer::get() called ⇣" << '\n'
        << "is read_ " << std::boolalpha << std::format("[ {} ]", get_read_()) << '\n'
        << "is allocated_ " << std::boolalpha << std::format("[ {} ]", get_allocated_() ) << '\n'
        << "is empty " << std::boolalpha << std::format("[ {} ]", nuts_buffer_.empty() ) << '\n';
    }
  }

  return nuts_buffer_;
}

nuts_buffer_unlined_t& buffer::get( const size_t& line ) {

  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock( mtx_ );
    if (DEBUG_BUFFER_CONST) {
      BUFFER << std::format("buffer::get(line[{}]) called ⇣", line) << '\n'
        << "is read_ " << std::boolalpha << std::format("[ {} ]", get_read_()) << '\n'
        << "is allocated_ " << std::boolalpha << std::format("[ {} ]", get_allocated_() ) << '\n'
        << "is empty " << std::boolalpha << std::format("[ {} ]", nuts_buffer_.empty() ) << '\n';
    }
  }

  return nuts_buffer_[ line ];
}

nuts_byte_t& buffer::get( const size_t& line, const size_t& col ) {

  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock( mtx_ );
    if (DEBUG_BUFFER_CONST) {
      BUFFER << std::format("buffer::get(line[{}], col[{}]) called ⇣", line, col) << '\n'
        << "is read_ " << std::boolalpha << std::format("[ {} ]", get_read_()) << '\n'
        << "is allocated_ " << std::boolalpha << std::format("[ {} ]", get_allocated_() ) << '\n'
        << "is empty " << std::boolalpha << std::format("[ {} ]", nuts_buffer_.empty() ) << '\n';
    }
  }

  return nuts_buffer_[ line ][ col ];
}

}
