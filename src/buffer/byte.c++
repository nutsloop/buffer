#include "buffer.h++"

namespace nutsloop {

char buffer::ch(const std::byte b) {
  { // MARK: (buffer) UNIQUE_LOCK
    std::unique_lock lock(mtx_);
    return static_cast<char>(nutsbyte_ = b);
  }
}

nuts_byte_t buffer::byte(char c) {

  { // MARK: (buffer) UNIQUE_LOCK
    std::unique_lock lock(mtx_);
    nuts_byte_.clear();
    nuts_byte_.push_back(static_cast<std::byte>(c));

    return nuts_byte_;
  }
}

nuts_byte_t buffer::byte(const std::string &str) {

  if (str.size() > 1) {
    throw std::invalid_argument("buffer::byte(const std::string &str): string size must be 1");
  }
  { // MARK: (buffer) UNIQUE_LOCK
    std::unique_lock lock(mtx_);
    nuts_byte_.clear();
    for (const unsigned char c : str) {
      nuts_byte_.push_back(static_cast<std::byte>(c));
    }

    return nuts_byte_;
  }
}

std::string buffer::byte(const nuts_byte_t &byte) {

  { // MARK: (buffer) UNIQUE_LOCK
    std::unique_lock lock(mtx_);
    nuts_byte_ = byte;
    return {reinterpret_cast<const char *>(nuts_byte_.data()), nuts_byte_.size()};
  }
}

} // namespace nutsloop
