#include "buffer.h++"

namespace nutsloop {

bool buffer::get_read_(){

#if DEBUG_BUFFER == true
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::get_read_() called ⇣" << '\n'
             << "  read_ -> [ " << std::boolalpha << read_ << " ]" << '\n';
    }
#endif

  return read_.load();
}

}
