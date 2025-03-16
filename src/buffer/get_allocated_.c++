#include "buffer.h++"

namespace nutsloop {

bool buffer::get_allocated_() const {

#if DEBUG_BUFFER == true
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      std::string allocated_string = allocated_.load() ? "true"_.blue().to_string() : "false"_.red().to_string();
      BUFFER << '\n' << "  buffer::get_allocated_() called ⇣"_.green().bold() << '\n'
             << ansi("    allocated_ -> [{}]", allocated_string) << " ]" << '\n';
    }
#endif

  return allocated_.load();
}

}
