#include "buffer.h++"

#include <fstream>
#include <sstream>
#include <unistd.h>

namespace nutsloop {

void buffer::read( const std::filesystem::path& file_path ) {

  if (DEBUG) {
    BUFFER << std::format("buffer::read([{}]) called ⇣", file_path.string()) << '\n';
  }

  if ( !std::filesystem::exists( file_path ) ) {
    if ( DEBUG ) {
      BUFFER << "  file do not exist. it will throw std::runtime_error" << '\n';
    }
    throw std::invalid_argument( "File does not exist" );
  }

  if ( get_allocated_() ) {
    nuts_buffer_unlined_.clear();
    nuts_buffer_unlined_.shrink_to_fit();
    nuts_buffer_.clear();
    nuts_buffer_.shrink_to_fit();
    unset_allocated_();
  }

  set_read_();
  // If the file size is 0, push the empty "unlined" buffer to the main buffer
  // and exit early, as there's nothing more to process.
  if (std::filesystem::file_size( file_path ) == 0) {
    if (DEBUG) {

      BUFFER << "  file size is 0. buffer is filled with one line and one null byte." << '\n';
    }
    nuts_buffer_unlined_.push_back( nuts_byte_ );
    nuts_buffer_.push_back( nuts_buffer_unlined_ );
    insert_metadata_( addr_hex_(), file_path, std::nullopt );
    return;
  }

  // Open the file for reading
  std::ifstream file( file_path, std::ios::binary );
  if ( !file.is_open() ) {
    if ( DEBUG ) {
      BUFFER << "  file could not be opened. it will throw std::runtime_error" << '\n';
    }
    throw std::runtime_error( "File could not be opened" );
  }

  for ( std::string line; std::getline( file, line ); ) {
    nuts_buffer_unlined_.clear();
    nuts_buffer_unlined_.reserve( line.size() );
    for ( const auto& c : line ) {
      nuts_buffer_unlined_.push_back( static_cast<nuts_byte_t>( c ) );
    }
    nuts_buffer_unlined_.push_back( nuts_byte_ );
    nuts_buffer_.push_back( nuts_buffer_unlined_ );
  }
  insert_metadata_( addr_hex_(), file_path, std::nullopt );

  if (DEBUG) {
    BUFFER << "  file is read. buffer is filled with the lines of the file." << '\n'
           << "metadata: " << '\n'
           << std::format("  mem_addr: {}", std::get<0>(metadata_)) << '\n'
           << std::format("  filename: {}", std::string(std::get<1>(metadata_).value_or( "nullopt" ))) << '\n'
           << std::format("  ident: {}", std::get<2>(metadata_).value_or( "nullopt" )) << '\n'
           << "  buffer size: " << nuts_buffer_.size() << '\n';

    // HINT: print the buffer content
    // std::cout << "buffer content: " << '\n';
    // int i = -1;
    // for ( const auto& line : nuts_buffer_ ) {
    //   std::cout << "  line: " << i++ << '\n';
    //   for ( const auto& c : line ) {
    //     std::cout << std::format( "[{}]", static_cast<u8>( c ) );
    //   }
    //   std::cout << '\n';
    // }
  }
}

void buffer::read( const std::filesystem::path& file_path, const int& fd, const off_t& file_size ) {

  if (DEBUG) {
    BUFFER << std::format("buffer::read(fd[{}], file_size[{}]) called ⇣", fd, file_size) << '\n';
  }

  if ( get_allocated_() ) {
    nuts_buffer_unlined_.clear();
    nuts_buffer_unlined_.shrink_to_fit();
    nuts_buffer_.clear();
    nuts_buffer_.shrink_to_fit();
    unset_allocated_();
  }

  set_read_();
  // If the file size is 0, push the empty "unlined" buffer to the main buffer
  // and exit early, as there's nothing more to process.
  if (file_size == 0) {
    if (DEBUG) {

      BUFFER << "  file size is 0. buffer is filled with one line and one null byte." << '\n';
    }
    nuts_buffer_unlined_.push_back( nuts_byte_ );
    nuts_buffer_.push_back( nuts_buffer_unlined_ );
    insert_metadata_( addr_hex_(), file_path, std::nullopt );
    return;
  }

  size_t offset = 0;
  constexpr size_t chunk_size = 1024; // Define chunk size for reads

  // Temporary buffer for reading
  std::vector<nuts_byte_t> temp_buffer( chunk_size );

  while ( offset < static_cast<size_t>( file_size ) ) {
    // Read data into temporary buffer
    const ssize_t bytes_read = ::read( fd, temp_buffer.data(), chunk_size );

    // Handle exceptions or errors
    if ( bytes_read < 0 ) {
      auto error = []() -> std::string {
        std::ostringstream oss;
        oss << "Error reading file: " << std::strerror(errno);
        return oss.str();
      };

      throw std::runtime_error(error());
    }

    // EOF reached
    if ( bytes_read == 0 ) {
      break;
    }

    // Process the read data
    for ( ssize_t i = 0; i < bytes_read; ++i ) {

      if ( nuts_byte_t byte = temp_buffer[ i ]; byte == nuts_byte_t{ '\n' } ) {
        // Push the current unlined buffer to the main buffer
        nuts_buffer_.push_back( nuts_buffer_unlined_ );
        nuts_buffer_unlined_.clear();
        nuts_buffer_unlined_.shrink_to_fit();
      }
      else {
        // Append byte to the unlined buffer
        nuts_buffer_unlined_.push_back( byte );
      }
    }

    offset += bytes_read;
  }

  // Handle any remaining data in nuts_buffer_unlined_
  if ( !nuts_buffer_unlined_.empty() ) {
    nuts_buffer_.push_back( nuts_buffer_unlined_ );
  }

  // Ensure at least one element is in the buffer if no newline-separated content was processed
  if (nuts_buffer_.empty()) {
    nuts_buffer_.push_back(nuts_buffer_unlined_);
  }

  insert_metadata_( addr_hex_(), file_path, std::nullopt );
  if (DEBUG) {
    BUFFER << "  file is read. buffer is filled with the lines of the file." << '\n'
           << "metadata: " << '\n'
           << std::format("  mem_addr: {}", std::get<0>(metadata_)) << '\n'
           << std::format("  filename: {}", std::string(std::get<1>(metadata_).value_or( "nullopt" ))) << '\n'
           << std::format("  ident: {}", std::get<2>(metadata_).value_or( "nullopt" )) << '\n'
           << "  buffer size: " << nuts_buffer_.size() << '\n';

    // HINT: print the buffer content
    // std::cout << "buffer content: " << '\n';
    // int i = -1;
    // for ( const auto& line : nuts_buffer_ ) {
    //   std::cout << "  line: " << i++ << '\n';
    //   for ( const auto& c : line ) {
    //     std::cout << std::format( "[{}]", static_cast<u8>( c ) );
    //   }
    //   std::cout << '\n';
    // }
  }
}

}
