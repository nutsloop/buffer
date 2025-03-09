#include "buffer.h++"

namespace nutsloop {

bool buffer::continuation_byte_(const std::string &str, const std::size_t offset, const std::size_t char_length) {

  for (std::size_t j = 1; j < char_length; ++j) {
    if (const auto cont = static_cast<unsigned char>(str[offset+ j]);
        (cont & 0xC0) != 0x80) { // Continuation byte must be 10xxxxxx.
      return false;
    }
  }

  return true;
}

}
