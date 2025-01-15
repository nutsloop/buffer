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

}
