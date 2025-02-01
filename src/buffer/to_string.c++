#include "buffer.h++"

namespace nutsloop {

std::string buffer::to_string() const {
  std::string buffer_string;
  for ( const auto& line : nuts_buffer_ ) {
    for ( const auto& byte : line ) {
      buffer_string.push_back( static_cast<char>( byte ) );
    }
    buffer_string.push_back( '\n' );
  }
  return buffer_string;
}

std::string buffer::to_string( const std::size_t line ) const {

  std::string buffer_string;
  for ( const auto& byte : nuts_buffer_[ line ] ) {
    buffer_string.push_back( static_cast<char>( byte ) );
  }
  return buffer_string;
}

std::string buffer::to_string( nuts_byte_t byte ) const {
  return {1,static_cast<char>(byte)};
}

}
