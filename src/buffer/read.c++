#include "buffer.h++"

#include <fstream>
#include <sstream>
#include <unistd.h>

namespace nutsloop {

void buffer::read(const std::filesystem::path &file_path) {
#if DEBUG_BUFFER == true
  BUFFER << std::format("buffer::read([{}]) called ⇣", file_path.string())
         << '\n';
#endif

  if (this->get_allocated_()) {
    throw std::runtime_error("buffer::read() called on buffer that is already allocated");
  }

  this->from_file_( file_path );
}
} // namespace nutsloop
