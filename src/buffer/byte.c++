#include "buffer.h++"

namespace nutsloop {

nuts_byte_t buffer::byte( char c ) { return nuts_byte_ = static_cast<std::byte>( c ); }

}
