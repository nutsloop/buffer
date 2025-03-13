#include "buffer.h++"

namespace nutsloop {

void buffer::unset_manual_() { manual_.exchange(false); }

} // namespace nutsloop
