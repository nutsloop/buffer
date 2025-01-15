#include "buffer.h++"

namespace nutsloop {

void buffer::debug_is_activated_() {
  if ( debug_activated_.load() ) {
    return;
  }

  debug_activate_();
}

}
