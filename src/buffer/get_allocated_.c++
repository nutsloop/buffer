#include "buffer.h++"

namespace nutsloop {

bool buffer::get_allocated_(){

  if ( DEBUG ) {
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::get_allocated_() called ⇣" << '\n'
             << "  read_ -> [ " << std::boolalpha << allocated_ << " ]" << '\n';
    }
  }

  return allocated_.load();
}

}
