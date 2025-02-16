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

  // MARK: work with stream methods
  // small tokenizer base on jsx file

  buffer buf_stream;
  buf_stream.read(path);
  // first, let's print the whole file using the location(line,column) and byte
  while (auto c = buf_stream.stream()) {

    const auto [location, byte] = *c;
    stream_log->ostream() << std::format("  line @ ({})  column@ ({}) byte -> ({})\n", location[0],
                                         location[1], static_cast<char>(byte))
                          << '\n';

    // like a streaming from remote or stuff like that
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // second, let's print a kind of tokenizer stuff using the stream capabilities
  while (auto c = buf_stream.stream()) {
    auto [location, byte] = *c;
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
      stream_log->ostream() << static_cast<char>(byte);
    }

    // let's play with the dot operator
    if (byte == buf_stream.byte('.')) {
      stream_log->ostream() << "DOT";
      continue;
    }

    // let's play with the parenthesis
    if (byte == buf_stream.byte('(')) {
      stream_log->ostream() << "LPAREN";
      continue;
    }
    if (byte == buf_stream.byte(')')) {
      stream_log->ostream() << "RPAREN";
      continue;
    }

    // let's play with brackets
    if (byte == buf_stream.byte('[')) {
      stream_log->ostream() << "LBRACKET";
      continue;
    }
    if (byte == buf_stream.byte(']')) {
      stream_log->ostream() << "RBRACKET";
      continue;
    }

    // let's play with curly braces
    if (byte == buf_stream.byte('{')) {
      stream_log->ostream() << "LBRACE";
      continue;
    }
    if (byte == buf_stream.byte('}')) {
      stream_log->ostream() << "RBRACE";
      continue;
    }

    // let's play with the comma operator
    if (byte == buf_stream.byte(',')) {
      stream_log->ostream() << "COMMA";
      continue;
    }

    // let's play with the colon operator
    if (byte == buf_stream.byte(':')) {
      stream_log->ostream() << "COLON";
      continue;
    }

    // let's play with the semicolon operator
    if (byte == buf_stream.byte(';')) {
      stream_log->ostream() << "SEMICOLON";
      continue;
    }

    // let's play with the plus operator
    if (byte == buf_stream.byte('+')) {
      stream_log->ostream() << "PLUS";
      continue;
    }

    // let's play with the minus operator
    if (byte == buf_stream.byte('-')) {
      stream_log->ostream() << "MINUS";
      continue;
    }

    // let's play with the asterisk operator
    if (byte == buf_stream.byte('*')) {
      stream_log->ostream() << "ASTERISK";
      continue;
    }

    // let's play with the slash operator
    if (byte == buf_stream.byte('/')) {
      stream_log->ostream() << "SLASH";
      continue;
    }

    // let's play with the percent operator
    if (byte == buf_stream.byte('%')) {
      stream_log->ostream() << "PERCENT";
      continue;
    }

    // let's play with the ampersand operator
    if (byte == buf_stream.byte('&')) {
      stream_log->ostream() << "AMPERSAND";
      continue;
    }

    // let's play with the pipe operator
    if (byte == buf_stream.byte('|')) {
      stream_log->ostream() << "PIPE";
      continue;
    }

    // let's play with the caret operator
    if (byte == buf_stream.byte('^')) {
      stream_log->ostream() << "CARET";
      continue;
    }

    // let's play with the tilde operator
    if (byte == buf_stream.byte('~')) {
      stream_log->ostream() << "TILDE";
      continue;
    }

    // let's play with the exclamation operator
    if (byte == buf_stream.byte('!')) {
      stream_log->ostream() << "EXCLAMATION";
      continue;
    }

    // let's play with the question operator
    if (byte == buf_stream.byte('?')) {
      stream_log->ostream() << "QUESTION";
      continue;
    }

    // let's play with the at operator
    if (byte == buf_stream.byte('@')) {
      stream_log->ostream() << "AT";
      continue;
    }

    // let's play with the hash operator
    if (byte == buf_stream.byte('#')) {
      stream_log->ostream() << "HASH";
      continue;
    }

    // let's play with the dollar operator
    if (byte == buf_stream.byte('$')) {

      stream_log->ostream() << "DOLLAR";
      continue;
    }

    // let's play with low-dash operator
    if (byte == buf_stream.byte('_')) {
      stream_log->ostream() << "_";
      continue;
    }

    // let's play with string single quotes
    if (byte == buf_stream.byte('\'')) {

      stream_log->ostream() << "S_QUOTE";
      x = x + 1;

      while (auto str = buf_stream.stream(y, x++, byte)) {
        buf_stream.set_stream_col(x);
        stream_log->ostream() << static_cast<char>(*str);
      }
      stream_log->ostream() << "S_QUOTE";
      buf_stream.set_stream_col(x);
      continue;
    }
    // let's play with string double quotes
    if (byte == buf_stream.byte('"')) {

      stream_log->ostream() << "D_QUOTE";
      x = x + 1;

      while (auto str = buf_stream.stream(y, x++, byte)) {
        buf_stream.set_stream_col(x);
        stream_log->ostream() << static_cast<char>(*str);
      }
      stream_log->ostream() << "D_QUOTE";
      buf_stream.set_stream_col(x);
      continue;
    }

    // let's play with the equal sign operator
    if (byte == buf_stream.byte('=')) {
      buf_stream.set_stream_col(x + 1);

      if (buf_stream.get(y, x + 1) == buf_stream.byte('=')) {

        if (buf_stream.get(y, x + 2) == buf_stream.byte('=')) {
          stream_log->ostream() << "STRICT_EQUAL";
          buf_stream.set_stream_col(x + 3);
          continue;
        }
        stream_log->ostream() << "LOOSE_EQUAL";
        buf_stream.set_stream_col(x + 2);
        continue;
      }
      // it may be an arrow function on the equal game.
      if (buf_stream.get(y, x + 1) == buf_stream.byte('>')) {
        stream_log->ostream() << "ARROW";
        buf_stream.set_stream_col(x + 2);
        continue;
      }
      stream_log->ostream() << "EQUAL";
    }
  }

  /*
   *FIXME: to use the stream in threads is necessary to create a registry of the streams
   *       or as per this example the streams will overlap
   */
  /*std::thread stream_thread_0( [&buf_stream]() {
    while ( auto c = buf_stream.stream() ) {
    auto [ location, byte ] = *c;
    auto [ y, x ] = location;

    // skip newlines
    if ( byte == buf_stream.byte( '\n' ) ) {
      continue;
    }

    // skip whitespace and nullbytes if any
    if ( byte == buf_stream.byte( ' ' ) || byte == buf_stream.byte( '\t' ) || byte ==
  buf_stream.byte( '\r' ) || byte == buf_stream.byte( '\0' ) || byte == buf_stream.byte( '\v' ) ||
  byte == buf_stream.byte( '\f' ) ) { continue;
    }

    // let's print everything that is alphanumeric
    if ( std::isalnum( static_cast<char>( byte ) ) ) {
      stream_log->ostream() << static_cast<char>( byte );
    }

    // let's play with the dot operator
    if ( byte == buf_stream.byte( '.' ) ) {
      stream_log->ostream() << "DOT";
      continue;
    }

    // let's play with the parenthesis
    if ( byte == buf_stream.byte( '(' ) ) {
      stream_log->ostream() << "LPAREN";
      continue;
    }
    if ( byte == buf_stream.byte( ')' ) ) {
      stream_log->ostream() << "RPAREN";
      continue;
    }

    // let's play with brackets
    if ( byte == buf_stream.byte( '[' ) ) {
      stream_log->ostream() << "LBRACKET";
      continue;
    }
    if ( byte == buf_stream.byte( ']' ) ) {
      stream_log->ostream() << "RBRACKET";
      continue;
    }

    // let's play with curly braces
    if ( byte == buf_stream.byte( '{' ) ) {
      stream_log->ostream() << "LBRACE";
      continue;
    }
    if ( byte == buf_stream.byte( '}' ) ) {
      stream_log->ostream() << "RBRACE";
      continue;
    }

    // let's play with the comma operator
    if ( byte == buf_stream.byte( ',' ) ) {
      stream_log->ostream() << "COMMA";
      continue;
    }

    // let's play with the colon operator
    if ( byte == buf_stream.byte( ':' ) ) {
      stream_log->ostream() << "COLON";
      continue;
    }

    // let's play with the semicolon operator
    if ( byte == buf_stream.byte( ';' ) ) {
      stream_log->ostream() << "SEMICOLON";
      continue;
    }

    // let's play with the plus operator
    if ( byte == buf_stream.byte( '+' ) ) {
      stream_log->ostream() << "PLUS";
      continue;
    }

    // let's play with the minus operator
    if ( byte == buf_stream.byte( '-' ) ) {
      stream_log->ostream() << "MINUS";
      continue;
    }

    // let's play with the asterisk operator
    if ( byte == buf_stream.byte( '*' ) ) {
      stream_log->ostream() << "ASTERISK";
      continue;
    }

    // let's play with the slash operator
    if ( byte == buf_stream.byte( '/' ) ) {
      stream_log->ostream() << "SLASH";
      continue;
    }

    // let's play with the percent operator
    if ( byte == buf_stream.byte( '%' ) ) {
      stream_log->ostream() << "PERCENT";
      continue;
    }

    // let's play with the ampersand operator
    if ( byte == buf_stream.byte( '&' ) ) {
      stream_log->ostream() << "AMPERSAND";
      continue;
    }

    // let's play with the pipe operator
    if ( byte == buf_stream.byte( '|' ) ) {
      stream_log->ostream() << "PIPE";
      continue;
    }

    // let's play with the caret operator
    if ( byte == buf_stream.byte( '^' ) ) {
      stream_log->ostream() << "CARET";
      continue;
    }

    // let's play with the tilde operator
    if ( byte == buf_stream.byte( '~' ) ) {
      stream_log->ostream() << "TILDE";
      continue;
    }

    // let's play with the exclamation operator
    if ( byte == buf_stream.byte( '!' ) ) {
      stream_log->ostream() << "EXCLAMATION";
      continue;
    }

    // let's play with the question operator
    if ( byte == buf_stream.byte( '?' ) ) {
      stream_log->ostream() << "QUESTION";
      continue;
    }

    // let's play with the at operator
    if ( byte == buf_stream.byte( '@' ) ) {
      stream_log->ostream() << "AT";
      continue;
    }

    // let's play with the hash operator
    if ( byte == buf_stream.byte( '#' ) ) {
      stream_log->ostream() << "HASH";
      continue;
    }

    // let's play with the dollar operator
    if ( byte == buf_stream.byte( '$' ) ) {

      stream_log->ostream() << "DOLLAR";
      continue;
    }

    // let's play with low-dash operator
    if ( byte == buf_stream.byte( '_' ) ) {
      stream_log->ostream() << "_";
      continue;
    }

    // let's play with string single quotes
    if ( byte == buf_stream.byte( '\'' ) ) {

      stream_log->ostream() << "S_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        stream_log->ostream() << static_cast<char>( *str );
      }
      stream_log->ostream() << "S_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }
    // let's play with string double quotes
    if ( byte == buf_stream.byte( '"' ) ) {

      stream_log->ostream() << "D_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        stream_log->ostream() << static_cast<char>( *str );
      }
      stream_log->ostream() << "D_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }

    // let's play with the equal sign operator
    if ( byte == buf_stream.byte( '=' ) ) {
      buf_stream.set_stream_col( x + 1 );

      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '=' ) ) {

        if ( buf_stream.get( y, x + 2 ) == buf_stream.byte( '=' ) ) {
          stream_log->ostream() << "STRICT_EQUAL";
          buf_stream.set_stream_col( x + 3 );
          continue;
        }
        stream_log->ostream() << "LOOSE_EQUAL";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      // it may be an arrow function on the equal game.
      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '>' ) ) {
        stream_log->ostream() << "ARROW";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      stream_log->ostream() << "EQUAL";
    }
  }
  });

  std::thread stream_thread_1( [&buf_stream]() {
    while ( auto c = buf_stream.stream() ) {
    auto [ location, byte ] = *c;
    auto [ y, x ] = location;

    // skip newlines
    if ( byte == buf_stream.byte( '\n' ) ) {
      continue;
    }

    // skip whitespace and nullbytes if any
    if ( byte == buf_stream.byte( ' ' ) || byte == buf_stream.byte( '\t' ) || byte ==
  buf_stream.byte( '\r' ) || byte == buf_stream.byte( '\0' ) || byte == buf_stream.byte( '\v' ) ||
  byte == buf_stream.byte( '\f' ) ) { continue;
    }

    // let's print everything that is alphanumeric
    if ( std::isalnum( static_cast<char>( byte ) ) ) {
      stream_log->ostream() << static_cast<char>( byte );
    }

    // let's play with the dot operator
    if ( byte == buf_stream.byte( '.' ) ) {
      stream_log->ostream() << "DOT";
      continue;
    }

    // let's play with the parenthesis
    if ( byte == buf_stream.byte( '(' ) ) {
      stream_log->ostream() << "LPAREN";
      continue;
    }
    if ( byte == buf_stream.byte( ')' ) ) {
      stream_log->ostream() << "RPAREN";
      continue;
    }

    // let's play with brackets
    if ( byte == buf_stream.byte( '[' ) ) {
      stream_log->ostream() << "LBRACKET";
      continue;
    }
    if ( byte == buf_stream.byte( ']' ) ) {
      stream_log->ostream() << "RBRACKET";
      continue;
    }

    // let's play with curly braces
    if ( byte == buf_stream.byte( '{' ) ) {
      stream_log->ostream() << "LBRACE";
      continue;
    }
    if ( byte == buf_stream.byte( '}' ) ) {
      stream_log->ostream() << "RBRACE";
      continue;
    }

    // let's play with the comma operator
    if ( byte == buf_stream.byte( ',' ) ) {
      stream_log->ostream() << "COMMA";
      continue;
    }

    // let's play with the colon operator
    if ( byte == buf_stream.byte( ':' ) ) {
      stream_log->ostream() << "COLON";
      continue;
    }

    // let's play with the semicolon operator
    if ( byte == buf_stream.byte( ';' ) ) {
      stream_log->ostream() << "SEMICOLON";
      continue;
    }

    // let's play with the plus operator
    if ( byte == buf_stream.byte( '+' ) ) {
      stream_log->ostream() << "PLUS";
      continue;
    }

    // let's play with the minus operator
    if ( byte == buf_stream.byte( '-' ) ) {
      stream_log->ostream() << "MINUS";
      continue;
    }

    // let's play with the asterisk operator
    if ( byte == buf_stream.byte( '*' ) ) {
      stream_log->ostream() << "ASTERISK";
      continue;
    }

    // let's play with the slash operator
    if ( byte == buf_stream.byte( '/' ) ) {
      stream_log->ostream() << "SLASH";
      continue;
    }

    // let's play with the percent operator
    if ( byte == buf_stream.byte( '%' ) ) {
      stream_log->ostream() << "PERCENT";
      continue;
    }

    // let's play with the ampersand operator
    if ( byte == buf_stream.byte( '&' ) ) {
      stream_log->ostream() << "AMPERSAND";
      continue;
    }

    // let's play with the pipe operator
    if ( byte == buf_stream.byte( '|' ) ) {
      stream_log->ostream() << "PIPE";
      continue;
    }

    // let's play with the caret operator
    if ( byte == buf_stream.byte( '^' ) ) {
      stream_log->ostream() << "CARET";
      continue;
    }

    // let's play with the tilde operator
    if ( byte == buf_stream.byte( '~' ) ) {
      stream_log->ostream() << "TILDE";
      continue;
    }

    // let's play with the exclamation operator
    if ( byte == buf_stream.byte( '!' ) ) {
      stream_log->ostream() << "EXCLAMATION";
      continue;
    }

    // let's play with the question operator
    if ( byte == buf_stream.byte( '?' ) ) {
      stream_log->ostream() << "QUESTION";
      continue;
    }

    // let's play with the at operator
    if ( byte == buf_stream.byte( '@' ) ) {
      stream_log->ostream() << "AT";
      continue;
    }

    // let's play with the hash operator
    if ( byte == buf_stream.byte( '#' ) ) {
      stream_log->ostream() << "HASH";
      continue;
    }

    // let's play with the dollar operator
    if ( byte == buf_stream.byte( '$' ) ) {

      stream_log->ostream() << "DOLLAR";
      continue;
    }

    // let's play with low-dash operator
    if ( byte == buf_stream.byte( '_' ) ) {
      stream_log->ostream() << "_";
      continue;
    }

    // let's play with string single quotes
    if ( byte == buf_stream.byte( '\'' ) ) {

      stream_log->ostream() << "S_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        stream_log->ostream() << static_cast<char>( *str );
      }
      stream_log->ostream() << "S_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }
    // let's play with string double quotes
    if ( byte == buf_stream.byte( '"' ) ) {

      stream_log->ostream() << "D_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        stream_log->ostream() << static_cast<char>( *str );
      }
      stream_log->ostream() << "D_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }

    // let's play with the equal sign operator
    if ( byte == buf_stream.byte( '=' ) ) {
      buf_stream.set_stream_col( x + 1 );

      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '=' ) ) {

        if ( buf_stream.get( y, x + 2 ) == buf_stream.byte( '=' ) ) {
          stream_log->ostream() << "STRICT_EQUAL";
          buf_stream.set_stream_col( x + 3 );
          continue;
        }
        stream_log->ostream() << "LOOSE_EQUAL";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      // it may be an arrow function on the equal game.
      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '>' ) ) {
        stream_log->ostream() << "ARROW";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      stream_log->ostream() << "EQUAL";
    }
  }
  });

  stream_thread_0.join();
  stream_thread_1.join();*/

  stream_log->flush();
  return EXIT_SUCCESS;
}
