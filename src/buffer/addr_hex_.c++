#include "buffer.h++"

namespace nutsloop {

uintptr_t buffer::addr_hex_() {
  return reinterpret_cast<uintptr_t>( nuts_buffer_.data());
}

}
