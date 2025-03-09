#include "buffer.h++"

namespace nutsloop {

nuts_buffer_t &buffer::get() {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    const ansi is_read = get_read_() ? "true"_.blue() : "false"_.red();
    const ansi is_from_string = get_from_string_() ? "true"_.blue() : "false"_.red();
    const ansi is_allocated = get_allocated_() ? "true"_.blue() : "false"_.red();
    const ansi is_empty = nuts_buffer_.empty() ? "true"_.blue() : "false"_.red();

    BUFFER << '\n'
           << "  buffer::get() called ⇣"_.green().bold() << '\n'
           << ansi("    is read_ -> [{}]", is_read.to_string()) << '\n'
           << ansi("    is from_string_ -> [{}]", is_from_string.to_string()) << '\n'
           << ansi("    is allocated_ -> [{}]", is_allocated.to_string()) << '\n'
           << ansi("    is empty -> [{}]", is_empty.to_string()) << '\n';
  }
#endif

  return nuts_buffer_;
}

nuts_buffer_unlined_t &buffer::get(const size_t &line) {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    const ansi is_read = get_read_() ? "true"_.blue() : "false"_.red();
    const ansi is_from_string = get_from_string_() ? "true"_.blue() : "false"_.red();
    const ansi is_allocated = get_allocated_() ? "true"_.blue() : "false"_.red();
    const ansi is_empty = nuts_buffer_.empty() ? "true"_.blue() : "false"_.red();
    BUFFER << '\n'
           << ansi("  buffer::get(line[{}]) called ⇣", line).green().bold() << '\n'
           << ansi("    is read_ -> [{}]", is_read.to_string()) << '\n'
           << ansi("    is from_string_ -> [{}]", is_from_string.to_string()) << '\n'
           << ansi("    is allocated_ -> [{}]", is_allocated.to_string()) << '\n'
           << ansi("    is empty -> [{}]", is_empty.to_string()) << '\n';
  }
#endif

  return nuts_buffer_[line];
}

nuts_byte_t &buffer::get(const size_t &line, const size_t &col) {

#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock(mtx_);
    const ansi is_read = get_read_() ? "true"_.blue() : "false"_.red();
    const ansi is_from_string = get_from_string_() ? "true"_.blue() : "false"_.red();
    const ansi is_allocated = get_allocated_() ? "true"_.blue() : "false"_.red();
    const ansi is_empty = nuts_buffer_.empty() ? "true"_.blue() : "false"_.red();
    BUFFER << '\n'
           << ansi("  buffer::get(line[{}],col[{}]) called ⇣", line, col).green().bold() << '\n'
           << ansi("    is read_ -> [{}]", is_read.to_string()) << '\n'
           << ansi("    is from_string_ -> [{}]", is_from_string.to_string()) << '\n'
           << ansi("    is allocated_ -> [{}]", is_allocated.to_string()) << '\n'
           << ansi("    is empty -> [{}]", is_empty.to_string()) << '\n';
  }
#endif

  return nuts_buffer_[line][col];
}

} // namespace nutsloop
