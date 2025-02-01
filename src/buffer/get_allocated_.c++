#include "buffer.h++"

namespace nutsloop {

bool buffer::get_allocated_(){

  if ( DEBUG_BUFFER_CONST ) {
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::get_allocated_() called ⇣" << '\n'
             << "  allocated_ -> [ " << std::boolalpha << allocated_ << " ]" << '\n';
    }
  }

  return allocated_.load();
}

}
