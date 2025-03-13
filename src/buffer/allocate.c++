#include "buffer.h++"

namespace nutsloop {

void buffer::allocate() {

  { // MARK (buffer) MUTEX LOCK

    std::shared_lock lock(mtx_);
    if (this->get_allocated_()) {
      throw std::runtime_error("buffer::allocate() called on buffer that is already allocated");
    }

    nuts_buffer_unlined_.push_back(null_byte_);
    nuts_buffer_.push_back(std::move(nuts_buffer_unlined_));
    nuts_buffer_unlined_.clear();
    metadata_ = nuts_buffer_metadata_t(addr_hex_(), std::nullopt, std::nullopt);
    set_allocated_();
    set_manual_();
  }
}

} // namespace nutsloop
