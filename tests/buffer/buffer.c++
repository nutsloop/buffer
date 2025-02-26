#include "buffer.h++"

#include "ansi.h++"

using namespace nutsloop;
int main() {

  log_settings_t settings("buffer_test", "buf_buffer.log", true, std::nullopt, std::nullopt);
  log::set(settings);
  log::activate();
  const auto buffer_log = nutsloop::log::get_instance("buffer_test");

  buffer_log->ostream() << "buffer constructors test"_.background_blue().bold() << std::endl;

  const std::string message =
      "hello world!\nbye bye\0"; // NOLINT(*-string-literal-with-embedded-nul)
  nutsloop::buffer buf_from_string(message);

  buffer_log->ostream()
      << '\n'
      << "using method buffer(const std::string &str)"_.background_magenta().bright_white().bold()
      << std::endl;

  auto buf_string_stream = buf_from_string.stream();

  while (auto stream = buf_string_stream.next()) {

    auto [location, byte] = *stream;
    [[maybe_unused]] auto [y, x] = location;

    if (byte == buf_from_string.byte('\n')) {
      buffer_log->ostream() << "N"_.red().bold();
      continue;
    }

    buffer_log->ostream() << ansi("{}", static_cast<char>(byte)).green().bold();
  }

  buffer_log->ostream()
      << '\n'
      << "expected the buffer to have size -> (1) despite of having a new line in the string"_
             .bold()
      << '\n'
      << ansi("buffer::size({})", buf_from_string.size()) << std::endl;

  const char *c_str = "hello world!\nbye bye";
  nutsloop::buffer buf_from_c_string(c_str);

  buffer_log->ostream()
      << '\n'
      << "using method buffer(const char *c_str)"_.background_magenta().bright_white().bold()
      << std::endl;

  auto buf_c_string_stream = buf_from_c_string.stream();

  while (auto stream = buf_c_string_stream.next()) {

    auto [location, byte] = *stream;
    [[maybe_unused]] auto [y, x] = location;

    if (byte == buf_from_c_string.byte('\n')) {
      buffer_log->ostream() << "N"_.red().bold();
      continue;
    }

    buffer_log->ostream() << ansi("{}", static_cast<char>(byte)).green().bold();
  }

  buffer_log->ostream()
      << '\n'
      << "expected the buffer to have size -> (1) despite of having a new line in the string"_
             .bold()
      << '\n'
      << ansi("buffer::size({})", buf_from_string.size()) << std::endl;

  char *c_str_ptr = new char[50];
  std::strcpy(c_str_ptr, "hello world!\nbye bye");
  nutsloop::buffer buf_from_c_string_ptr(c_str);

  buffer_log->ostream()
      << '\n'
      << "using method buffer(char *c_str)"_.background_magenta().bright_white().bold()
      << std::endl;

  auto buf_c_string_ptr_stream = buf_from_c_string_ptr.stream();

  while (auto stream = buf_c_string_ptr_stream.next()) {

    auto [location, byte] = *stream;
    [[maybe_unused]] auto [y, x] = location;

    if (byte == buf_from_c_string_ptr.byte('\n')) {
      buffer_log->ostream() << "N"_.red().bold();
      continue;
    }

    buffer_log->ostream() << ansi("{}", static_cast<char>(byte)).green().bold();
  }

  buffer_log->ostream()
      << '\n'
      << "expected the buffer to have size -> (1) despite of having a new line in the string"_
             .bold()
      << '\n'
      << ansi("buffer::size({})", buf_from_string.size()) << std::endl;
  buffer_log->flush();
  return EXIT_SUCCESS;
}
