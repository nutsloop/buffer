#include "buffer.h++"

namespace nutsloop {

std::size_t buffer::size() const { return nuts_buffer_.size(); }

std::size_t buffer::size( const std::size_t line ) const { return nuts_buffer_[ line ].size(); }

}
