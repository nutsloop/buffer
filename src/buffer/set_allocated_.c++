#include "buffer.h++"

namespace nutsloop {

void buffer::set_allocated_() {

  const bool previous_allocated = allocated_.exchange( true );
#if DEBUG_BUFFER == true
    {// MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::set_allocated_() called ⇣" << '\n'
       << "  allocated_ ("
       << " was -> [ " << std::boolalpha << previous_allocated << " ]" // previous
       << " => now[ " << std::boolalpha << allocated_ << " ] )" << '\n'; // actual
    }
#endif
}

}
