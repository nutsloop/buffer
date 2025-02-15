#include "buffer/internal_debug.h++"

#include "log.h++"

std::atomic<bool> temporary_flag_destructor_{false};
namespace nutsloop::nbuffer {

#if DEBUG_BUFFER == true

internal_debug::internal_debug() {

  log_settings_t log_settings("buffer", "buffer.log", true, std::nullopt, std::nullopt);
  log::set(log_settings);
  log::activate();

  BUFFER << "internal debug ( \"buffer\" ) activated..." << '\n'
         << "  buffer log_setting_t ⇣" << '\n'
         << "    ident: " << log_settings.get_ident() << '\n'
         << "    filename: " << log_settings.get_filename() << '\n'
         << "    active: " << std::boolalpha << log_settings.get_active() << '\n';
}

internal_debug::~internal_debug() {

  if (std::filesystem::file_size( log::get_absolute_path( "buffer" ) ) > 0) {
    BUFFER_NO_HEAD << '\n' << "internal debug ( \"buffer\" ) flushing..." << '\n';
    if (log::is_open( "buffer" )) {
      BUFFER_NO_HEAD << "internal debug ( \"buffer\" ) closing..." << '\n';
    }
    log::flush("buffer");
  }

  if ( log::is_open( "buffer" )) {
    log::close("buffer");
  }
}

#endif

} // namespace nutsloop::nbuffer
