#include "buffer.h++"

namespace nutsloop {

std::string buffer::addr_hex_() {
  return std::format( "0x{:x}", reinterpret_cast<uintptr_t>( nuts_buffer_.data() ) );
}

}
