#include "buffer.h++"

#include "ansi.h++"

#include <cstdlib>

using namespace nutsloop::nansi;

int main() {

  nutsloop::log_settings_t settings("size", "buf_size.log", true, std::nullopt, std::nullopt);
  log::set(settings);
  log::activate();
  const auto size_log = log::get_instance("size");

  constexpr auto path = "/Volumes/a-ssd/CODE/nutsloop/jsx/index.jsx";
  nutsloop::buffer buf;
  buf.read(path);

  size_log->ostream() << "buffer size test"_.background_blue().bold() << std::endl;

  // check first the size of the buffer
  size_log->ostream() << "retrieve buffer size => "_.bold()
                      << std::format("buf.size() = {}", buf.size()) << std::endl;

  // check the size of line 8
  size_log->ostream() << "retrieve buffer line 8 size => "_.bold()
                      << std::format("buf.size(8) = {}", buf.size(8)) << std::endl;

  // check the size of line 8 removing null byte
  size_log->ostream() << "retrieve buffer line 8 size without null byte => "_.bold()
                      << std::format("buf.size_line_strip_null_byte(8) = {}",
                                     buf.size(8,true))
                      << std::endl;

  // check the sizes of the buffer
  size_log->ostream() << "retrieve buffer sizes => "_.bold()
                      << std::format("buf.sizes() = {}", buf.sizes()) << std::endl;

  size_log->ostream() << "retrieve buffer sizes stripping null byte off => "_.bold()
                    << std::format("buf.sizes() = {}", buf.sizes(true)) << std::endl;

  // check the size exceeding the buffer size
  try {
    size_log->ostream() << "causing exception buf.size(20)"_.bold() << std::endl;
    [[maybe_unused]] std::size_t failed_size = buf.size(20);
  } catch (std::out_of_range &e) {
    size_log->ostream()
        << ansi("throw std::out_of_range -> msg => [{}]", e.what()).red().bold().underline()
        << std::endl;
  }

  // check the size exceeding the buffer size
  try {
    size_log->ostream() << "causing exception buf.size(20, true) NOT includes null byte"_.bold()
                        << std::endl;
    [[maybe_unused]] std::size_t failed_size = buf.size(20, true);
  } catch (std::out_of_range &e) {
    size_log->ostream()
        << ansi("throw std::out_of_range -> msg => [{}]", e.what()).red().bold().underline()
        << std::endl;
  }

  size_log->flush();

  return EXIT_SUCCESS;
}
