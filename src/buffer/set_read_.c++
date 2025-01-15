#include "buffer.h++"

namespace nutsloop {

void buffer::set_read_() {

  const bool previous_read = read_.exchange( true );
  if ( DEBUG ) {
    {// MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      BUFFER << "buffer::set_read_() called ⇣" << '\n'
       << "  read_ ("
       << " was -> [ " << std::boolalpha << previous_read << " ]" // previous
       << " => now[ " << std::boolalpha << read_ << " ] )" << '\n'; // actual
    }
  }
}

}
