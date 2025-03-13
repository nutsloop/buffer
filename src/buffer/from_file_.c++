#include "buffer.h++"

#include <fstream>

namespace nutsloop {

void buffer::from_file_(const std::filesystem::path &file_path) {

  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock(mtx_);

    // Open the file for reading
    std::ifstream file(file_path, std::ios::binary | std::ios::in);

    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file for reading.");
    }

    for (std::string line; std::getline(file, line);) {

      nuts_buffer_unlined_.clear();
      nuts_buffer_unlined_.reserve(line.size());

      // Insert chars from the line into the nuts_buffer_unlined
      process_bytes_(line);

      nuts_buffer_unlined_.push_back(null_byte_);
      nuts_buffer_.emplace_back(std::move(nuts_buffer_unlined_));
      nuts_buffer_unlined_.clear();
      nuts_buffer_unlined_.shrink_to_fit();
      nuts_byte_.clear();
      nuts_byte_.shrink_to_fit();
    }

    file.close();
    metadata_ = nuts_buffer_metadata_t(addr_hex_(), file_path, std::nullopt);
    set_allocated_();
    set_read_();
  }
}

}
