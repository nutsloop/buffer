#include "buffer.h++"

namespace nutsloop {

std::atomic<bool> buffer::stream_active_{false};
std::atomic<std::size_t> buffer::stream_line_{0};
std::atomic<std::size_t> buffer::stream_col_{0};
std::atomic<nuts_byte_t> buffer::stream_byte_{nuts_byte_t{0x00}};
// HINT: not implemented yet.
std::unique_ptr<nuts_stream_registry_t> buffer::stream_registry_{nullptr};

buffer::buffer() {
#if DEBUG_BUFFER == true
  set_internal_debug_();
  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock(mtx_);
    BUFFER << "buffer::buffer() called ⇣" << '\n'
           << "  Initializes an empty buffer instance without allocating any "
              "resources."
           << '\n';
  }
#endif
}

buffer::buffer(const bool has_registry) {
#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

  if (has_registry) {
    set_has_registry_();
    if (registry_ == nullptr) {
      registry_ = std::make_unique<nuts_buffer_registry_t>();
    }
  }

#if DEBUG_BUFFER == true
  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock(mtx_);
    BUFFER << std::format("buffer::buffer([{}]) called ⇣",
                          has_registry ? "true" : "false")
           << '\n'
           << "  Initializes an empty buffer instance without allocating any "
              "resources."
           << '\n'
           << "  has_registry -> [ " << std::boolalpha << has_registry << " ]"
           << '\n';
  }
#endif
}

} // namespace nutsloop
