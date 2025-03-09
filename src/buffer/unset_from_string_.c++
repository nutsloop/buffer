#include "buffer.h++"

namespace nutsloop {

void buffer::unset_from_string_() { string_.exchange(false); }
}
