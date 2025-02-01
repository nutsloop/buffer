#include "buffer.h++"

namespace nutsloop {

void buffer::debug_activate_() {

#if DEBUG_BUFFER
  const bool previous_debug_activated = debug_activated_.exchange( true );

    const log_settings_t log_settings(
    "buffer",
    "buffer.log",
    true,
    std::nullopt,
    std::nullopt
    );
    log::activate();
    log::set( log_settings );

    BUFFER << "log::full_running( \"buffer\" ) -> [ false ]" << '\n'
           << "  log::activate() will now be called." << '\n'
           << "  buffer log_setting_t ⇣" << '\n'
           << "    ident: " << log_settings.ident << '\n'
           << "    filename: " << log_settings.filename << '\n'
           << "    active: " << std::boolalpha << log_settings.active << '\n';


  BUFFER << "buffer::debug_activate_() called ⇣" << '\n'
  << "  debug_activated_ ("
  << " was -> [ " << std::boolalpha << previous_debug_activated << " ]" // previous
  << " => now[ " << std::boolalpha << debug_activated_ << " ] )" << '\n'; // actual

#endif
}

}
