#include "buffer.h++"

namespace nutsloop {

void buffer::unset_from_string_() { from_string_.exchange(false); }
}
