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

buffer::buffer(const std::string &str) {
#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

  this->from_string_(str);
}

buffer::buffer(const std::filesystem::path &file_path) {

#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

#if DEBUG_BUFFER == true
  BUFFER
      << '\n'
      << ansi("  buffer::buffer(file_path[{}]) called ⇣", file_path.string()).green().bold() << '\n'
      << "    initializing a buffer reading a file at the given path."_.magenta()
      << '\n';
#endif

  this->from_file_(file_path);
}

} // namespace nutsloop
