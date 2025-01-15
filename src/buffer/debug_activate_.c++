#include "buffer.h++"

namespace nutsloop {

void buffer::debug_activate_() {

  const bool previous_debug_activated = debug_activated_.exchange( true );
  if ( !log::full_running( "buffer" ) ) {

    const log_settings_t log_settings{
    .ident = "buffer",
    .filename = "buffer.log",
    .active = true,
  };
    log::activate();
    log::set( log_settings );

    BUFFER << "log::full_running( \"buffer\" ) -> [ false ]" << '\n'
           << "  log::activate() will now be called." << '\n'
           << "  buffer log_setting_t ⇣" << '\n'
           << "    ident: " << log_settings.ident << '\n'
           << "    filename: " << log_settings.filename << '\n'
           << "    active: " << std::boolalpha << log_settings.active << '\n';
  }else {
    BUFFER << "log::full_running( \"buffer\" ) -> [ true ]" << '\n'
           << "  log::activate() will not be called." << '\n';
  }

  BUFFER << "buffer::debug_activate_() called ⇣" << '\n'
  << "  debug_activated_ ("
  << " was -> [ " << std::boolalpha << previous_debug_activated << " ]" // previous
  << " => now[ " << std::boolalpha << debug_activated_ << " ] )" << '\n'; // actual

}

}
