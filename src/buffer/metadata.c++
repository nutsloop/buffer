#include "buffer.h++"

namespace nutsloop {

void buffer::insert_metadata_( const nuts_buffer_mem_addr_t& mem_addr, const nuts_buffer_from_file_t& filename, const nuts_buffer_registry_identifier_t& ident ) {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock( mtx_ );
    BUFFER << "buffer::insert_metadata_() called ⇣" << '\n'
           << "  mem_addr: " << mem_addr << '\n'
           << "  filename: " << filename.value_or( "nullopt" ) << '\n'
           << "  ident: " << ident.value_or( "nullopt" ) << '\n';
  }
#endif

  metadata_ = std::make_tuple( mem_addr, filename, ident );
}

nuts_buffer_metadata_t &buffer::get_metadata() {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    std::string buf_address = std::get<0>( metadata_ );
    std::string file_path = std::get<1>( metadata_ ).value_or( "empty" );
    std::string registry = std::get<2>( metadata_ ).value_or( "empty" );
    BUFFER << '\n'
           << "  buffer::get_metadata() called ⇣"_.green().bold() << '\n'
           << ansi("    metadata_.buffer_address -> [{}]", buf_address).magenta() << '\n'
           << ansi("    metadata_.file_path -> [{}]", file_path).magenta() << '\n'
           << ansi("    metadata_.registry -> [{}]", registry).magenta()
           << '\n';
  }
#endif

  return metadata_;
}

std::string buffer::get_metadata_buffer_address(){

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::unique_lock lock(mtx_);
    std::string buf_address = std::get<0>(metadata_);
    BUFFER << '\n'
           << "  buffer::get_metadata_buffer_address() called ⇣"_.green().bold() << '\n'
           << ansi("    metadata_.buffer_address -> [{}]", buf_address).magenta() << '\n';
  }
#endif;

  return std::get<0>(metadata_);
}

std::string buffer::get_metadata_file_path() {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    std::string file_path = std::get<1>( metadata_ ).value_or( "empty" );
    BUFFER << '\n'
           << "  buffer::get_metadata_buffer_address() called ⇣"_.green().bold() << '\n'
           << ansi("    metadata_.file_path -> [{}]", file_path).magenta() << '\n';
  }
#endif

  return std::get<1>( metadata_ ).value_or( "empty" );

}

std::string buffer::get_metadata_registry_identifier() {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    std::string registry_identifier = std::get<2>( metadata_ ).value_or( "empty" );
    BUFFER << '\n'
           << "  buffer::get_metadata_registry_identifier called ⇣"_.green().bold() << '\n'
           << ansi("    metadata_.registry_identifier -> [{}]", registry_identifier).magenta() << '\n';
  }
#endif

  return std::get<2>( metadata_ ).value_or( "empty" );

}


} // namespace nutsloop
