#include "buffer.h++"

namespace nutsloop {

buffer::buffer() {
#if DEBUG_BUFFER == true
  set_internal_debug_();
  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock(mtx_);
    BUFFER << '\n'
           << "  buffer::buffer() called ⇣"_.green().bold() << '\n'
           << "    Initializing an empty buffer instance without allocating any resources." << '\n';
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
    BUFFER << std::format("buffer::buffer([{}]) called ⇣", has_registry ? "true" : "false") << '\n'
           << "  Initializes an empty buffer instance without allocating any "
              "resources."
           << '\n'
           << "  has_registry -> [ " << std::boolalpha << has_registry << " ]" << '\n';
  }
#endif
}

} // namespace nutsloop
