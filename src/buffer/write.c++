#include "buffer.h++"

#include <fstream>
#include <iostream>

namespace nutsloop {

void buffer::write(const std::optional<std::filesystem::path> &file_path) {

  std::filesystem::path path;
  if (file_path.has_value()) {
    path = file_path.value();
  } else {
    if (metadata().path_to_file.has_value()) {
      path = metadata().path_to_file.value();
    } else {
      path = std::filesystem::temp_directory_path() / "nutsloop_buffer";
    }
  }

  std::ofstream file(path, std::ios::binary);
  for (const auto &lines : nuts_buffer_) {
    for (const auto &bytes : lines) {
      if (bytes != byte('\0')) {
        file.write(reinterpret_cast<const char *>(bytes.data()),
                   static_cast<std::streamsize>(bytes.size()));
      }
    }
  }

  std::cout << "Wrote " << path << std::endl;
  file.close();
}

} // namespace nutsloop
