#include "buffer.h++"

namespace nutsloop {

void buffer::set_has_registry_() {

  [[maybe_unused]]const bool previous_has_registry = has_registry_.exchange( true );
#if DEBUG_BUFFER == true
    {// MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::set_has_registry_() called ⇣" << '\n'
       << "  has_registry_ ("
       << " was -> [ " << std::boolalpha << previous_has_registry << " ]" // previous
       << " => now[ " << std::boolalpha << has_registry_ << " ] )" << '\n'; // actual
    }
#endif
}

}
