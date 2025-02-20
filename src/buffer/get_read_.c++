#include "buffer.h++"

namespace nutsloop {

bool buffer::get_read_(){

#if DEBUG_BUFFER == true
    { // MARK: (buffer) MUTEX_LOCK
      std::shared_lock lock( mtx_ );
      std::string read_string = read_.load() ? "true"_.blue().to_string() : "false"_.red().to_string();
      BUFFER << '\n' << "  buffer::get_read_() called ⇣"_.green().bold() << '\n'
             << ansi("    read_ -> [{}]", read_string) << " ]" << '\n';
    }
#endif

  return read_.load();
}

}
