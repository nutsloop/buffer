#pragma once

#include <array>
#include <string>
#include <string_view>

namespace nutsloop::ncolor {
class color {
public:

  explicit color( const std::string_view text ) : text_( text ) {}
  ~color() = default;

  // MARK (ARGS_COLOR) red
  [[nodiscard]] color red() const { return color( "\033[31m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_red() const { return color( "\033[91m" + text_ + "\033[0m" ); }
  [[nodiscard]] color background_red() const { return color( "\033[41m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_background_red() const { return color( "\033[101m" + text_ + "\033[0m" ); }
  // MARK (ARGS_COLOR) green
  [[nodiscard]] color green() const { return color( "\033[32m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_green() const { return color( "\033[92m" + text_ + "\033[0m" ); }
  [[nodiscard]] color background_green() const { return color( "\033[42m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_background_green() const { return color( "\033[102m" + text_ + "\033[0m" ); }
  // MARK (ARGS_COLOR) yellow
  [[nodiscard]] color yellow() const { return color( "\033[33m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_yellow() const { return color( "\033[93m" + text_ + "\033[0m" ); }
  [[nodiscard]] color background_yellow() const { return color( "\033[43m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_background_yellow() const { return color( "\033[103m" + text_ + "\033[0m" ); }
  // MARK (ARGS_COLOR) blue
  [[nodiscard]] color blue() const { return color( "\033[34m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_blue() const { return color( "\033[94m" + text_ + "\033[0m" ); }
  [[nodiscard]] color background_blue() const { return color( "\033[44m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_background_blue() const { return color( "\033[104m" + text_ + "\033[0m" ); }
  // MARK (ARGS_COLOR) magenta
  [[nodiscard]] color magenta() const { return color( "\033[35m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_magenta() const { return color( "\033[95m" + text_ + "\033[0m" ); }
  [[nodiscard]] color background_magenta() const { return color( "\033[45m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_background_magenta() const { return color( "\033[105m" + text_ + "\033[0m" ); }
  // MARK (ARGS_COLOR) cyan
  [[nodiscard]] color cyan() const { return color( "\033[36m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_cyan() const { return color( "\033[96m" + text_ + "\033[0m" ); }
  [[nodiscard]] color background_cyan() const { return color( "\033[46m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_background_cyan() const { return color( "\033[106m" + text_ + "\033[0m" ); }
  // MARK (ARGS_COLOR) gray|white
  [[nodiscard]] color white() const { return color( "\033[37m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_white() const { return color( "\033[97m" + text_ + "\033[0m" ); }
  [[nodiscard]] color background_white() const { return color( "\033[47m" + text_ + "\033[0m" ); }
  [[nodiscard]] color bright_background_white() const { return color( "\033[107m" + text_ + "\033[0m" ); }
  // MARK (ARGS_COLOR) rgb
  [[nodiscard]] color rgb( const std::array<int, 3>& rgb ) const {
    return color(
      "\033[38;2;" + std::to_string( rgb[ 0 ] ) + ";" +
      std::to_string( rgb[ 1 ] ) + ";" +
      std::to_string( rgb[ 2 ] ) + "m" + text_ + "\033[0m" );
  }
  [[nodiscard]] color background_rgb( const std::array<int, 3>& rgb ) const {
    return color(
      "\033[48;2;" + std::to_string( rgb[ 0 ] ) + ";" +
      std::to_string( rgb[ 1 ] ) + ";" +
      std::to_string( rgb[ 2 ] ) + "m" + text_ + "\033[0m" );
  }
  // MARK (ARGS_COLOR) 256 color code
  [[nodiscard]] color color256( const int color_code ) const {
    return color( "\033[38;5;" + std::to_string( color_code ) + "m" + text_ + "\033[0m" );
  }
  [[nodiscard]] color background_color256( const int color_code ) const {
    return color( "\033[48;5;" + std::to_string( color_code ) + "m" + text_ + "\033[0m" );
  }

  [[nodiscard]] color bold() const { return color( "\033[1m" + text_ + "\033[0m" ); }
  [[nodiscard]] color underline() const { return color( "\033[4m" + text_ + "\033[0m" ); }
  [[nodiscard]] color blink() const { return color( "\033[5m" + text_ + "\033[0m" ); }
  [[nodiscard]] color reverse() const { return color( "\033[7m" + text_ + "\033[0m" ); }
  [[nodiscard]] color hidden() const { return color( "\033[8m" + text_ + "\033[0m" ); }
  friend std::ostream& operator<<( std::ostream& os, const color& c_str );
  explicit operator std::string() const { return text_; }
  [[nodiscard]] std::string to_string() const {
    return text_;
  }

private:
  std::string text_;
};

inline color operator""_s( const char* str, std::size_t ) {
  return color( std::string_view( str ) );
}

inline std::ostream& operator<<( std::ostream& os, const color& c_str ) {
  os << c_str.text_;
  return os;
}

}
