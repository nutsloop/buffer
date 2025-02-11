#include "buffer.h++"

namespace nutsloop {

bool buffer::get_has_registry_() {

#if DEBUG_BUFFER == true
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::get_has_registry_() called ⇣" << '\n'
             << "  has_registry_ -> [ " << std::boolalpha << has_registry_ << " ]" << '\n';
    }
#endif

  return has_registry_.load();
}

}
