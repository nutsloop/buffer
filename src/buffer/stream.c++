#include "buffer.h++"

namespace nutsloop {

buffer::stream buffer::buffer_stream() {
  return stream(this);
}

} // namespace nutsloop
