#include "buffer.h++"

namespace nutsloop {

void buffer::string(const std::string &str) {

  if (this->get_allocated_()) {
    throw std::runtime_error("buffer::string() called on buffer that is already allocated");
  }

  this->from_string_(str);
}

}
