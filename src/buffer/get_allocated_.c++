#include "buffer.h++"

namespace nutsloop {

bool buffer::get_allocated_(){

#if DEBUG_BUFFER == true
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::get_allocated_() called ⇣" << '\n'
             << "  allocated_ -> [ " << std::boolalpha << allocated_ << " ]" << '\n';
    }
#endif

  return allocated_.load();
}

}
