#include "buffer.h++"

namespace nutsloop {

void buffer::reset(const bool registry/*=true*/) {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    BUFFER << '\n' << "  buffer::reset() called ⇣"_.green().bold() << '\n';
  }
#endif

  nuts_buffer_unlined_.clear();
  nuts_buffer_.clear();
  nutsbyte_ = std::byte{0x00};
  unset_from_string_();
  unset_read_();
  unset_manual_();
  if (registry) {
    unset_has_registry_();
  }
  unset_allocated_();
}

} // namespace nutsloop
