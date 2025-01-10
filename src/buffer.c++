#include "buffer.h++"

namespace nutsloop {

buffer::buffer() {
  if ( DEBUG ) {
    { // MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      this->debug_is_activated_();
      BUFFER << "buffer::buffer() called ⇣" << '\n'
             << "  Initializes an empty buffer instance without allocating any resources." << '\n';
    }
  }
}

buffer::buffer( const bool has_registry ) {

  if ( has_registry ) {
    set_has_registry_();
    if ( registry_ == nullptr) {
      registry_ = std::make_unique<nuts_buffer_registry_t>();
    }
  }

  if ( DEBUG ) {
    { // MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      this->debug_is_activated_();
      BUFFER << std::format( "buffer::buffer([{}]) called ⇣", has_registry ? "true" : "false" ) << '\n'
             << "  Initializes an empty buffer instance without allocating any resources." << '\n'
             << "  has_registry -> [ " << std::boolalpha << has_registry << " ]" << '\n';
    }
  }
}

// void buffer::read( const std::filesystem::path& file_path ) {
//
//   if (DEBUG) {
//     BUFFER << std::format("buffer::read([{}]) called ⇣") << '\n';
//   }
//
//   if ( !std::filesystem::exists( file_path ) ) {
//     if (DEBUG) {
//       BUFFER << "  file do not exist. it will throw std::runtime_error" << '\n';
//     }
//     throw std::runtime_error( "File does not exist" );
//   }
//
//
//
//   // If the file size is 0, push the empty "unlined" buffer to the main buffer
//   // and exit early, as there's nothing more to process.
//   if (file_size == 0) {
//     nuts_buffer_.push_back(nuts_buffer_unlined_);
//     return;
//   }
//
//   size_t offset = 0;
//   constexpr size_t chunk_size = 1024; // Define chunk size for reads
//
//   // Temporary buffer for reading
//   std::vector<nuts_byte_t> temp_buffer( chunk_size );
//   std::vector<nuts_byte_t> line;
//   std::vector<std::vector<nuts_byte_t>> lines;
//   std::vector<int> line_length;
//
//   while ( offset < static_cast<size_t>( file_size ) ) {
//     // Read data into temporary buffer
//     const ssize_t bytes_read = ::read( fd, temp_buffer.data(), chunk_size );
//
//     // Handle exceptions or errors
//     if ( bytes_read < 0 ) {
//       perror( "Error reading file" );
//       break;
//     }
//
//     // EOF reached
//     if ( bytes_read == 0 ) {
//       break;
//     }
//
//     // Process the read data
//     for ( ssize_t i = 0; i < bytes_read; ++i ) {
//
//       if ( nuts_byte_t byte = temp_buffer[ i ]; byte == nuts_byte_t{ '\n' } ) {
//         // Push the current unlined buffer to the main buffer
//         nuts_buffer_.push_back( nuts_buffer_unlined_ );
//         nuts_buffer_unlined_.clear();
//       }
//       else {
//         // Append byte to the unlined buffer
//         nuts_buffer_unlined_.push_back( byte );
//       }
//     }
//
//     offset += bytes_read;
//   }
//
//   // Handle any remaining data in nuts_buffer_unlined_
//   if ( !nuts_buffer_unlined_.empty() ) {
//     nuts_buffer_.push_back( nuts_buffer_unlined_ );
//   }
//
//   // Ensure at least one element is in the buffer if no newline-separated content was processed
//   if (nuts_buffer_.empty()) {
//     nuts_buffer_.push_back(nuts_buffer_unlined_);
//   }
// }

std::string buffer::addr_hex_() {
  return std::format( "0x{:x}", reinterpret_cast<uintptr_t>( nuts_buffer_.data() ) );
}

void buffer::insert_metadata_( const nuts_buffer_mem_addr_t& mem_addr, const nuts_buffer_from_file_t& filename, const nuts_buffer_registry_identifier_t& ident ) {

  if ( DEBUG ) {
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::insert_metadata_() called ⇣" << '\n'
             << "  mem_addr: " << mem_addr << '\n'
             << "  filename: " << filename.value_or( "nullopt" ) << '\n'
             << "  ident: " << ident.value_or( "nullopt" ) << '\n';
    }
  }

  metadata_ = std::make_tuple( mem_addr, filename, ident );
}

bool buffer::get_has_registry_() {

  if ( DEBUG ) {
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::get_has_registry_() called ⇣" << '\n'
             << "  has_registry_ -> [ " << std::boolalpha << has_registry_ << " ]" << '\n';
    }
  }

  return has_registry_.load();
}

void buffer::set_has_registry_() {

  const bool previous_has_registry = has_registry_.exchange( true );
  if ( DEBUG ) {
    {// MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::set_has_registry_() called ⇣" << '\n'
       << "  has_registry_ ("
       << " was -> [ " << std::boolalpha << previous_has_registry << " ]" // previous
       << " => now[ " << std::boolalpha << has_registry_ << " ] )" << '\n'; // actual
    }
  }
}

void buffer::debug_is_activated_() {
  if ( debug_activated_.load() ) {
    return;
  }

  debug_activate_();
}

void buffer::debug_activate_() {

  const bool previous_debug_activated = debug_activated_.exchange( true );
  const log_settings_t log_settings{
    .ident = "buffer",
    .filename = "buffer.log",
    .active = true,
  };

  log::activate();
  log::set( log_settings );

  BUFFER << "buffer::debug_activate_() called ⇣" << '\n'
  << "  debug_activated_ ("
  << " was -> [ " << std::boolalpha << previous_debug_activated << " ]" // previous
  << " => now[ " << std::boolalpha << debug_activated_ << " ] )" << '\n'; // actual

  BUFFER << "buffer log_setting_t ⇣" << '\n'
    << "  ident: " << log_settings.ident << '\n'
    << "  filename: " << log_settings.filename << '\n'
    << "  active: " << std::boolalpha << log_settings.active << '\n';

}
}
