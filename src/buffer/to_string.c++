#include "buffer.h++"

namespace nutsloop {

std::string buffer::to_string() const {
  std::string buffer_string;
  for ( const auto& line : nuts_buffer_ ) {
    for ( const auto& byte : line ) {
      // last null byte of the nuts_buffer_unlined_ is not included
      if (byte != nuts_byte_t{0x00}) {
        buffer_string.push_back( static_cast<char>( byte ) );
      }
    }
    buffer_string.push_back( '\n' );
  }
  return buffer_string;
}

std::string buffer::to_string( const std::size_t line ) const {

  std::string buffer_string;
  for ( const auto& byte : nuts_buffer_[ line ] ) {
    // last null byte of the nuts_buffer_unlined_ is not included
    if (byte != nuts_byte_t{0x00}) {
      buffer_string.push_back( static_cast<char>( byte ) );
    }
  }
  return buffer_string;
}

std::string buffer::to_string( const std::size_t line, const std::size_t col ) const {
  return {1,static_cast<char>(nuts_buffer_[ line ][ col ])};
}

}
