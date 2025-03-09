#include "buffer.h++"

namespace nutsloop {

bool buffer::get_from_string_() const { return string_.load(); }

} // namespace nutsloop
