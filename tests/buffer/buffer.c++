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

  // TODO: change the buffer types to this for handling multy byte stuff
  // ✅ UTF-8 Character Example: '😎' (U+1F60E) → 4 Bytes

  // ✅ ASCII Example: "Hi!"
  std::vector<std::byte> ascii_H = { std::byte{0x48} };  // 'H'
  std::vector<std::byte> ascii_i = { std::byte{0x69} };  // 'i'
  std::vector<std::byte> ascii_ex = { std::byte{0x21} }; // '!'

  // ✅ UTF-8 Example (Needs to be stored together as a vector)
  std::vector<std::byte> utf8_vector = { std::byte{0xF0}, std::byte{0x9F}, std::byte{0x98}, std::byte{0x8E} };

  // ✅ Organizing Into Nested Buffer Structure
  std::vector<std::vector<std::byte>> ascii_group = { ascii_H, ascii_i, ascii_ex }; // ASCII characters separately
  std::vector<std::vector<std::byte>> utf8_group = { utf8_vector }; // UTF-8 grouped together

  std::vector<std::vector<std::vector<std::byte>>> buffer = { ascii_group, utf8_group };

  // ✅ Printing Buffer Content
  buffer_log->ostream() << "Buffer Content:\n";
  for (const auto& group : buffer) {
    for (const auto& char_bytes : group) {
      for (std::byte b : char_bytes) {
        buffer_log->ostream()<< std::hex << std::to_integer<int>(b) << " ";
      }
      buffer_log->ostream() << "| ";
    }
    buffer_log->ostream() << "\n";
  }

  std::string result;

  for (const auto& group : buffer) {
    for (const auto& char_bytes : group) {
      for (std::byte b : char_bytes) {
        result += static_cast<char>(std::to_integer<int>(b));  // Convert bytes back to chars
      }
    }
  }

  buffer_log->ostream() << result;

  buffer_log->flush();
  return EXIT_SUCCESS;
}
