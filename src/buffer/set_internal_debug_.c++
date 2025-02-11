#include "buffer.h++"

namespace nutsloop {

#if DEBUG_BUFFER == true

void buffer::set_internal_debug_() {

  if ( internal_debug_ == nullptr) {
    internal_debug_ = std::make_unique<nbuffer::internal_debug>();
  }

}


#endif

}
