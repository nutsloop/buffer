#include "buffer.h++"

namespace nutsloop {

std::string buffer::to_string() const {
  std::string buffer_string;
  for (const auto &line : nuts_buffer_) {
    for (const auto &bytes : line) {
      // last null byte of the nuts_buffer_unlined_ is not included
      if (bytes != null_byte_) {
        for (const auto &byte : bytes) {
          buffer_string.push_back(static_cast<char>(byte));
        }
      }
    }
    buffer_string.push_back('\n');
  }
  return buffer_string;
}

std::string buffer::to_string(const std::size_t line) const {

  std::string line_string;
  for (const auto &bytes : nuts_buffer_[line]) {
    // last null byte of the nuts_buffer_unlined_ is not included
    if (bytes != null_byte_) {
      for (const auto &byte : bytes) {
        line_string.push_back(static_cast<char>(byte));
      }
    }
  }
  return line_string;
}

std::string buffer::to_string(const std::size_t line, const std::size_t col) const {
  // TODO check if the selected bytes aren't null_byte
  std::string byte_string;
  for (const nuts_byte_t &bytes = nuts_buffer_[line][col]; const auto &byte : bytes) {
    byte_string.push_back(static_cast<char>(byte));
  }
  return byte_string;
}

} // namespace nutsloop
