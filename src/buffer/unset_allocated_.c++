#include "buffer.h++"

namespace nutsloop {

void buffer::unset_allocated_() {

  const bool previous_allocated = allocated_.exchange( false );
  if ( DEBUG_BUFFER_CONST ) {
    {// MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::unset_allocated_() called ⇣" << '\n'
       << "  allocated_ ("
       << " was -> [ " << std::boolalpha << previous_allocated << " ]" // previous
       << " => now[ " << std::boolalpha << allocated_ << " ] )" << '\n'; // actual
    }
  }
}

}
