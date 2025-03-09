#include "buffer.h++"

namespace nutsloop {

std::size_t buffer::char_length_(const unsigned char &byte){

  std::size_t char_len = 0;

  // Determine expected length of UTF-8 sequence.
  if (byte < 0x80) {
    // 1-byte character (ASCII)
    char_len = 1;
  } else if ((byte & 0xE0) == 0xC0) {
    // 2-byte character
    char_len = 2;
  } else if ((byte & 0xF0) == 0xE0) {
    // 3-byte character
    char_len = 3;
  } else if ((byte & 0xF8) == 0xF0) {
    // 4-byte character
    char_len = 4;
  } else {
    // Malformed starting byte
    char_len = -1;
  }

  return char_len;
}

}
