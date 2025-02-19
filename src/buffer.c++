#include "buffer.h++"

namespace nutsloop {

buffer::buffer() {
#if DEBUG_BUFFER == true
  set_internal_debug_();
  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock(mtx_);
    BUFFER
        << '\n'
        << "  buffer::buffer() called ⇣"_.green().bold() << '\n'
        << "    initializing an empty buffer instance without allocating any resources."_.magenta()
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
    const ansi has_registry_string = has_registry ? "true"_.blue().bold() : "false"_.red().bold();
    BUFFER
        << '\n'
        << ansi("  buffer::buffer([{}]) called ⇣", has_registry).green().bold() << '\n'
        << "    initializing an empty buffer instance without allocating any resources."_.magenta()
        << '\n'
        << "    has_registry -> [ " << has_registry_string << " ]"_.white().bold() << '\n';
  }
#endif
}

} // namespace nutsloop
