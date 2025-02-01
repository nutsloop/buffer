#include "buffer.h++"

namespace nutsloop {

void buffer::insert_metadata_( const nuts_buffer_mem_addr_t& mem_addr, const nuts_buffer_from_file_t& filename, const nuts_buffer_registry_identifier_t& ident ) {

  if ( DEBUG_BUFFER_CONST ) {
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

}
