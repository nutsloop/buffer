#include "buffer/internal_debug.h++"

#include "log.h++"

namespace nutsloop::nbuffer {

#if DEBUG_BUFFER == true

internal_debug::internal_debug() {

  log_settings_t log_settings("buffer", "buffer.log", true, std::nullopt,
                              std::nullopt);
  log::set(log_settings);
  log::activate();

  BUFFER << "internal debug ( \"buffer\" ) activated..." << '\n'
         << "  buffer log_setting_t ⇣" << '\n'
         << "    ident: " << log_settings.get_ident() << '\n'
         << "    filename: " << log_settings.get_filename() << '\n'
         << "    active: " << std::boolalpha << log_settings.get_active()
         << '\n';
}

internal_debug::~internal_debug() {

  // TODO: once log library as the close method remember to close the file first
  BUFFER_NO_HEAD << '\n' << "internal debug ( \"buffer\" ) deactivated..." << '\n';
  log::flush("buffer");
}

#endif

} // namespace nutsloop::nbuffer
