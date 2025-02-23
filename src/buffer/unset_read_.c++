#include "buffer.h++"

namespace nutsloop {

void buffer::unset_read_() { read_.exchange(false); }

}
