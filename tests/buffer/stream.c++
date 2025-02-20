#include "buffer.h++"

#include "ansi.h++"

#include <iostream>
#include <thread>

/*

the JavaScript file used for this example.
save it and edit the path location accordingly

```javascript
function component(props) {

  this.handleButtonClick = () => {
    console.log('button clicked');
  }

  const d_quoted_string = "this is a double quote string";
  const x = 7, y = 8;

  if( 7 === y ){}

  if( x == 11 ){}

  return (
    <>
      <div className={'hello'}></div>
    </>
  )
}
```
*/

using namespace nutsloop;
int main() {

  log_settings_t settings("stream_test", "buf_stream.log", true, std::nullopt, std::nullopt);
  log::set(settings);
  log::activate();
  const auto stream_log = log::get_instance("stream_test");

  stream_log->ostream() << "buffer stream test"_.background_blue().bold() << std::endl;

  const auto path = std::getenv("JSX_FILE_BUFFER");
  if (path == nullptr) {
    std::cerr << "JSX_FILE_BUFFER environment variable not set" << std::endl;
    exit(1);
  }

  /*buffer buf_stream;
  buf_stream.read(path);
  stream_log->ostream()
      << "iterating the nuts_buffer_stream_t returned from buffer::stream()"_.bold() << '\n'
      << "alias for type"_.green().underline()
      << " -> std::optional<std::tuple<std::array<std::size_t, 2>, std::byte>>"_.yellow().bold()
      << '\n'
      << "stream doesn't return the null_byte"_.magenta().underline() << '\n';

  while (auto nuts_buffer_stream = buf_stream.stream(true)) {

    const auto [location, byte] = *nuts_buffer_stream;
    const auto [y, x] = location;
    if (x == 0) {
      stream_log->ostream()
          << "------------------------------------------------------"_.bold().bright_white()
          << '\n';
    }
    stream_log->ostream() << ansi("  line@({})", y).yellow().bold()
                          << ansi("  column@({})", x).magenta().bold()
                          << ansi("  byte->({})",
                                  (byte == buf_stream.byte('\n')
                                       ? "\\n"_.green().to_string()
                                       : ansi(buf_stream.to_string(byte)).green().to_string()))
                                 .bold()
                          << '\n';

    // like a streaming from remote or stuff like that
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }*/

  buffer buf_stream;
  buf_stream.read(path);
  stream_log->ostream()
      << "iterating the nuts_buffer_stream_t returned from buffer::stream::next()"_.bold() << '\n'
      << "alias for type"_.green().underline()
      << " -> std::optional<std::tuple<std::array<std::size_t, 2>, std::byte>>"_.yellow().bold()
      << '\n'
      << "stream doesn't return the null_byte"_.magenta().underline() << '\n';


  auto stream = buf_stream.make_stream();
  while (auto nuts_buffer_stream = stream.next(true)) {

    const auto [location, byte] = *nuts_buffer_stream;
    const auto [y, x] = location;
    if (x == 0) {
      stream_log->ostream()
          << "------------------------------------------------------"_.bold().bright_white()
          << '\n';
    }
    stream_log->ostream() << ansi("  line@({})", y).yellow().bold()
                          << ansi("  column@({})", x).magenta().bold()
                          << ansi("  byte->({})",
                                  (byte == buf_stream.byte('\n')
                                       ? "\\n"_.green().to_string()
                                       : ansi(buf_stream.to_string(byte)).green().to_string()))
                                 .bold()
                          << '\n';

    // like a streaming from remote or stuff like that
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // MARK: work with stream methods
  // small tokenizer base on jsx file
  auto tokenizer = buf_stream.make_stream();
  stream_log->ostream()
      << '\n'
      << "iterating the nuts_buffer_stream_t returned from buffer::stream::next()"_.bold() << '\n'
      << "alias for type"_.green().underline()
      << " -> std::optional<std::tuple<std::array<std::size_t, 2>, std::byte>>"_.yellow().bold()
      << '\n'
      << "stream doesn't return the null_byte"_.magenta().underline() << '\n'
      << "tokenizing the stream of the JSX file."_.bold().bright_background_blue() << '\n';


  while (auto nuts_buffer_stream = tokenizer.next(true)) {
    auto [location, byte] = *nuts_buffer_stream;
    auto [y, x] = location;

    // skip newlines
    if (byte == buf_stream.byte('\n')) {
      continue;
    }


    // skip whitespace and nullbytes if any
    if (byte == buf_stream.byte(' ') || byte == buf_stream.byte('\t') ||
        byte == buf_stream.byte('\r') || byte == buf_stream.byte('\0') ||
        byte == buf_stream.byte('\v') || byte == buf_stream.byte('\f')) {
      continue;
    }

    // let's print everything that is alphanumeric
    if (std::isalnum(static_cast<char>(byte))) {
      stream_log->ostream() << ansi(buf_stream.to_string(byte)).green().to_string();
    }

    // let's play with the dot operator
    if (byte == buf_stream.byte('.')) {
      stream_log->ostream() << "DOT"_.magenta();
      continue;
    }

    // let's play with the parenthesis
    if (byte == buf_stream.byte('(')) {
      stream_log->ostream() << "LPAREN"_.magenta();
      continue;
    }
    if (byte == buf_stream.byte(')')) {
      stream_log->ostream() << "RPAREN"_.magenta();
      continue;
    }

    // let's play with brackets
    if (byte == buf_stream.byte('[')) {
      stream_log->ostream() << "LBRACKET"_.magenta();
      continue;
    }
    if (byte == buf_stream.byte(']')) {
      stream_log->ostream() << "RBRACKET"_.magenta();
      continue;
    }

    // let's play with curly braces
    if (byte == buf_stream.byte('{')) {
      stream_log->ostream() << "LBRACE"_.magenta();
      continue;
    }
    if (byte == buf_stream.byte('}')) {
      stream_log->ostream() << "RBRACE"_.magenta();
      continue;
    }

    // let's play with the comma operator
    if (byte == buf_stream.byte(',')) {
      stream_log->ostream() << "COMMA"_.magenta().bold();
      continue;
    }

    // let's play with the colon operator
    if (byte == buf_stream.byte(':')) {
      stream_log->ostream() << "COLON"_.magenta();
      continue;
    }

    // let's play with the semicolon operator
    if (byte == buf_stream.byte(';')) {
      stream_log->ostream() << "SEMICOLON"_.magenta().bold();
      continue;
    }

    // let's play with the plus operator
    if (byte == buf_stream.byte('+')) {
      stream_log->ostream() << "PLUS"_.magenta();
      continue;
    }

    // let's play with the minus operator
    if (byte == buf_stream.byte('-')) {
      stream_log->ostream() << "MINUS"_.magenta();
      continue;
    }

    // let's play with the asterisk operator
    if (byte == buf_stream.byte('*')) {
      stream_log->ostream() << "ASTERISK"_.magenta();
      continue;
    }

    // let's play with the slash operator
    if (byte == buf_stream.byte('/')) {
      stream_log->ostream() << "SLASH"_.magenta();
      continue;
    }

    // let's play with the percent operator
    if (byte == buf_stream.byte('%')) {
      stream_log->ostream() << "PERCENT"_.magenta();
      continue;
    }

    // let's play with the ampersand operator
    if (byte == buf_stream.byte('&')) {
      stream_log->ostream() << "AMPERSAND"_.magenta();
      continue;
    }

    // let's play with the pipe operator
    if (byte == buf_stream.byte('|')) {
      stream_log->ostream() << "PIPE"_.magenta();
      continue;
    }

    // let's play with the caret operator
    if (byte == buf_stream.byte('^')) {
      stream_log->ostream() << "CARET"_.magenta();
      continue;
    }

    // let's play with the tilde operator
    if (byte == buf_stream.byte('~')) {
      stream_log->ostream() << "TILDE"_.magenta();
      continue;
    }

    // let's play with the exclamation operator
    if (byte == buf_stream.byte('!')) {
      stream_log->ostream() << "EXCLAMATION"_.magenta();
      continue;
    }

    // let's play with the question operator
    if (byte == buf_stream.byte('?')) {
      stream_log->ostream() << "QUESTION"_.magenta();
      continue;
    }

    // let's play with the at operator
    if (byte == buf_stream.byte('@')) {
      stream_log->ostream() << "AT"_.magenta();
      continue;
    }

    // let's play with the hash operator
    if (byte == buf_stream.byte('#')) {
      stream_log->ostream() << "HASH"_.magenta();
      continue;
    }

    // let's play with the dollar operator
    if (byte == buf_stream.byte('$')) {

      stream_log->ostream() << "DOLLAR"_.magenta();
      continue;
    }

    // let's play with low-dash operator
    if (byte == buf_stream.byte('_')) {
      stream_log->ostream() << "_"_.magenta();
      continue;
    }

    // let's play with string single quotes
    if (byte == buf_stream.byte('\'')) {

      stream_log->ostream() << "S_QUOTE"_.yellow();
      x = x + 1;

      while (auto str = tokenizer.next(y, x++, byte)) {
        tokenizer.move_at_column(x);
        stream_log->ostream() << static_cast<char>(*str);
      }
      stream_log->ostream() << "S_QUOTE"_.yellow();
      tokenizer.move_at_column(x);
      continue;
    }
    // let's play with string double quotes
    if (byte == buf_stream.byte('"')) {

      stream_log->ostream() << "D_QUOTE"_.blue();
      x = x + 1;

      while (auto str = tokenizer.next(y, x++, byte)) {
        tokenizer.move_at_column(x);
        stream_log->ostream() << static_cast<char>(*str);
      }
      stream_log->ostream() << "D_QUOTE"_.blue();
      tokenizer.move_at_column(x);
      continue;
    }

    // let's play with the equal sign operator
    if (byte == buf_stream.byte('=')) {
      tokenizer.move_at_column(x + 1);

      if (buf_stream.get(y, x + 1) == buf_stream.byte('=')) {

        if (buf_stream.get(y, x + 2) == buf_stream.byte('=')) {
          stream_log->ostream() << "STRICT_EQUAL"_.black().background_cyan();
          tokenizer.move_at_column(x + 3);
          continue;
        }
        stream_log->ostream() << "LOOSE_EQUAL"_.black().background_red();
        tokenizer.move_at_column(x + 2);
        continue;
      }
      // it may be an arrow function on the equal game.
      if (buf_stream.get(y, x + 1) == buf_stream.byte('>')) {
        stream_log->ostream() << "ARROW"_.black().background_magenta();
        tokenizer.move_at_column(x + 2);
        continue;
      }
      stream_log->ostream() << "EQUAL"_.black().background_yellow();
    }
  }

  /*
   *FIXME: to use the stream in threads is necessary to create a registry of the streams
   *       or as per this example the streams will overlap
   */

  stream_log->flush();
  stream_log->close();
  return EXIT_SUCCESS;
}
