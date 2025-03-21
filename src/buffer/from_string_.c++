#include "buffer.h++"

namespace nutsloop {

void buffer::from_string_(const std::string &str) {

  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock(mtx_);
    nuts_buffer_unlined_.reserve(str.size());

    process_bytes_(str);

    nuts_buffer_unlined_.push_back(null_byte_);
    nuts_buffer_.emplace_back(std::move(nuts_buffer_unlined_));
    nuts_buffer_unlined_.clear();
    nuts_byte_.clear();

    metadata_ = nuts_buffer_metadata_t(addr_hex_(), std::nullopt, std::nullopt);
    set_allocated_();
    set_from_string_();
  }
}

}
