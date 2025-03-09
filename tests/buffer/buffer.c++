#include "buffer.h++"

#include "ansi.h++"

#include <iostream>
#include <thread>

using namespace nutsloop;

int main() {

  log_settings_t settings("buffer_test", "buf_buffer.log", true, std::nullopt, std::nullopt);
  log::set(settings);
  log::activate();
  const auto buffer_log = nutsloop::log::get_instance("buffer_test");

  buffer_log->ostream() << "buffer constructors test"_.background_blue().bold() << std::endl;
  buffer_log->ostream() << '\n';

  // MARK: buffer(const std::string &str)
  buffer_log->ostream()
      << "using method buffer(const std::string &str)"_.background_magenta().bright_white().bold()
      << std::endl;

  const std::string message =
      "🥸 é € hello world!\nbye bye!\xC0"; // NOLINT(*-string-literal-with-embedded-nul)
  buffer buf_from_string(message);
  auto buf_from_string_stream = buf_from_string.stream();
  while (auto stream = buf_from_string_stream.next()) {
    auto [location, byte] = *stream;
    [[maybe_unused]] auto [y, x] = location;
    buffer_log->ostream() << buf_from_string.byte(byte);
  }

  buffer_log->ostream() << "\n\n";

  // MARK: buffer(const std::filesystem::path &file_path, bool use_fd = false)
  buffer_log->ostream() << "using method buffer(const std::filesystem::path &file_path)"_
                               .background_magenta()
                               .bright_white()
                               .bold()
                        << std::endl;

  std::filesystem::path path = std::getenv("TEST_FILE_BUFFER");
  if (path.empty()) {
    std::cerr << "TEST_FILE_BUFFER environment variable not set" << std::endl;
    exit(1);
  }

  buffer buf_from_file(path);
  auto buf_from_file_stream = buf_from_file.stream();
  while (auto stream = buf_from_file_stream.next()) {
    auto [location, byte] = *stream;
    [[maybe_unused]] auto [y, x] = location;
    buffer_log->ostream() << buf_from_file.byte(byte);
  }

  buffer_log->flush();
  return EXIT_SUCCESS;
}
