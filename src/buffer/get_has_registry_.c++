#include "buffer.h++"

namespace nutsloop {

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

}
