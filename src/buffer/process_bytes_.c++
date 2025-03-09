#include "buffer.h++"

namespace nutsloop {

void buffer::process_bytes_(const std::string &str) {

  for (std::size_t i = 0; i < str.size(); ++i) {
    // Clear temporary storage for the current sequence.
    nuts_byte_.clear();

    const std::size_t char_len = char_length_( static_cast<unsigned char>(str[i]) );
    if (char_len == -1) {
      // Malformed starting byte: insert U+FFFD.
      nuts_buffer_unlined_.push_back(malformed_byte_);
      ++i;
      continue;
    }

    // Ensure there are enough bytes remaining.
    if (i + char_len > str.size()) {
      // Not enough bytes: treat as malformed.
      nuts_buffer_unlined_.push_back(malformed_byte_);
      break;
    }

    // If valid, push the entire character sequence.
    for (std::size_t j = 0; j < char_len; ++j) {
      nuts_byte_.push_back(static_cast<std::byte>(str[i + j]));
    }

    if (!continuation_byte_( str, i, char_len )) {
      // Replace malformed sequence with U+FFFD.
      nuts_buffer_unlined_.push_back(malformed_byte_);
      // Skip the offending byte.
      ++i;
      continue;
    }

    // If valid, push the entire character sequence.

    nuts_buffer_unlined_.push_back(nuts_byte_);
    i += char_len - 1; // Advance the index appropriately.
  }

}


}
