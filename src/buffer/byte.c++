#include "buffer.h++"

namespace nutsloop {

nuts_byte_t buffer::byte(char c) { return nuts_byte_ = static_cast<nuts_byte_t>(c); }

char buffer::byte(const nuts_byte_t b) { return static_cast<char>(nuts_byte_ = b); }

} // namespace nutsloop
