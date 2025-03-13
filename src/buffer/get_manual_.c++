#include "buffer.h++"

namespace nutsloop {

bool buffer::get_manual_() const {
#if DEBUG_BUFFER == true
  { // MARK: (buffer) MUTEX_LOCK
    std::shared_lock lock( mtx_ );
    std::string manual_string = manual_.load() ? "true"_.blue().to_string() : "false"_.red().to_string();
    BUFFER << '\n' << "  buffer::get_manual_() called ⇣"_.green().bold() << '\n'
           << ansi("    manual_ -> [{}]", manual_string) << " ]" << '\n';
  }
#endif

  return manual_.load();
}

} // namespace nutsloop
