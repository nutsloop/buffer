#include "buffer.h++"

#include "ansi.h++"

using namespace nutsloop;
int main() {

  log_settings_t settings("buffer_test", "buf_buffer.log", true, std::nullopt,
                                    std::nullopt);
  log::set(settings);
  log::activate();
  const auto buffer_log = nutsloop::log::get_instance("buffer_test");

  buffer_log->ostream() << "buffer constructors test"_.background_blue().bold() << std::endl;

  buffer_log->flush();
  return EXIT_SUCCESS;
}
