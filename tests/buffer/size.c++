#include "buffer.h++"

#include <cstdlib>

int main() {

  nutsloop::log_settings_t settings("size", "buf_size.log", true, std::nullopt, std::nullopt);
  log::set(settings);
  log::activate();
  const auto size_log = log::get_instance("size");

  constexpr auto path = "/Volumes/a-ssd/CODE/nutsloop/jsx/index.jsx";
  nutsloop::buffer buf;
  buf.read(path);

  // check first the size of the buffer
  size_log->ostream() << std::format("buf.size() = {}", buf.size())
                          << std::endl;
  // check the size of line 8
  size_log->ostream() << std::format("buf.size(8) = {}", buf.size(8))
                          << std::endl;

  // check the size of line 8 removing null byte
  size_log->ostream() << std::format("buf.size_line_strip_null_byte(8) = {}", buf.size_line_strip_null_byte(8))
                        << std::endl;

  // check the size exceeding the buffer size
  try {
    size_log->ostream() << std::format("buf.size(20) = {}", buf.size(20))
                            << std::endl;
  } catch (std::out_of_range &e) {
    size_log->ostream() << "std::out_of_range" << std::endl;
    size_log->ostream() << e.what() << std::endl;
  }

  size_log->flush();

  return EXIT_SUCCESS;
}
