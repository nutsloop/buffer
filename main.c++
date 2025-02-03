#include "buffer.h++"

#include <iostream>
#include <thread>

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

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

#define BUF log::stream( "buf" )
int main() {

  constexpr nutsloop::u8 lines = 3;
  constexpr nutsloop::u16 bytes_per_line = 20;
  constexpr auto path = "/Volumes/a-ssd/CODE/nutsloop/jsx/index.jsx";

  nutsloop::buffer buffer_empty;
  buffer_empty.allocate( nutsloop::u8{ 24 }, nutsloop::u16{ 100 } );

  // creating a buffer with a registry
  nutsloop::buffer buf( true );
  buf.allocate_into( "main_buffer" );
  buf.allocate_into( "another_buffer" );
  buf.allocate_into( "yet_another_buffer with allocation", lines, bytes_per_line );

  nutsloop::buffer buf_fromfile;
  buf_fromfile.read( path );

  nutsloop::buffer buf_fromfd;

  const int fd_ = ::open( path, O_RDWR | O_EXCL, 0644 );

  // TODO handle exception
  if ( fd_ < 0 && errno != EEXIST ) {
    throw std::runtime_error( "Failed to open file" );
  }

  if ( fd_ < 0 && errno == ENOENT ) {
    throw std::runtime_error( "Failed to open file" );
  }

  struct stat file_stats{};
  fstat( fd_, &file_stats );
  const auto file_size = file_stats.st_size;

  buf_fromfd.read( std::filesystem::path{ path }, fd_, file_size );

  BUFFER << std::format( "buf_fromfd before removing line 2: size -> {}", buf_fromfd.size() ) << '\n';

  buf_fromfd >> buf_fromfd.delete_at( 1 );

  BUFFER << std::format( "buf_fromfd after removing line 2: size -> {}", buf_fromfd.size() ) << '\n';
  BUFFER << std::format( "buf_fromfd line 3 size -> {}", buf_fromfd.size( 2 ) ) << '\n';
  BUFFER << buf_fromfd.to_string( 2 ) << '\n';

  buf_fromfd >> buf_fromfd.delete_at( 2, 29 );
  buf_fromfd >> buf_fromfd.delete_at( 2, 29 );

  BUFFER << std::format( "buf_fromfd line 3 size -> {}", buf_fromfd.size( 3 ) ) << '\n';
  BUFFER << buf_fromfd.to_string( 2 ) << '\n';

  buf_fromfd << std::make_tuple( 2, 29, buf_fromfd.byte( '!' ) );
  buf_fromfd << std::make_tuple( 2, 29, buf_fromfd.byte( '!' ) );

  BUFFER << std::format( "buf_fromfd line 3 size -> {}", buf_fromfd.size( 2 ) ) << '\n';
  BUFFER << buf_fromfd.to_string( 2 ) << '\n';

  // Simulate uploading byte by byte
  while ( auto byte_opt = buf_fromfd.stream() ) {
    auto [ loc_, byte ] = *byte_opt;
    std::cout << static_cast<char>( byte ) << std::flush;
    // std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
  }

  while ( auto byte_opt = buf_fromfd.stream( 2 ) ) {
    auto [ _loc, byte ] = *byte_opt;
    std::cout << static_cast<char>( byte ) << std::flush;
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
  }

  ::close( fd_ );

  // MARK: work with stream methods
  // small tokenizer base on jsx file

  buffer buf_stream;
  buf_stream.read( path );
  const nutsloop::log_settings_t settings( "buf", "buf.log", true, std::nullopt, std::nullopt );

  log::set( settings );

  // first, let's print the whole file using the location(line,column) and byte
  while ( auto c = buf_stream.stream() ) {

    const auto [ location, byte ] = *c;
    BUF << std::format( "  line @ ({})  column@ ({}) byte -> ({})\n", location[ 0 ], location[ 1 ],
                        static_cast<char>( byte ) )
        << '\n';

    // like a streaming from remote or stuff like that
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
  }

  // second, let's print a kind of tokenizer stuff using the stream capabilities
  while ( auto c = buf_stream.stream() ) {
    auto [ location, byte ] = *c;
    auto [ y, x ] = location;

    // skip newlines
    if ( byte == buf_stream.byte( '\n' ) ) {
      continue;
    }

    // skip whitespace and nullbytes if any
    if ( byte == buf_stream.byte( ' ' ) || byte == buf_stream.byte( '\t' ) || byte == buf_stream.byte( '\r' ) ||
         byte == buf_stream.byte( '\0' ) || byte == buf_stream.byte( '\v' ) || byte == buf_stream.byte( '\f' ) ) {
      continue;
    }

    // let's print everything that is alphanumeric
    if ( std::isalnum( static_cast<char>( byte ) ) ) {
      BUF << static_cast<char>( byte );
    }

    // let's play with the dot operator
    if ( byte == buf_stream.byte( '.' ) ) {
      BUF << "DOT";
      continue;
    }

    // let's play with the parenthesis
    if ( byte == buf_stream.byte( '(' ) ) {
      BUF << "LPAREN";
      continue;
    }
    if ( byte == buf_stream.byte( ')' ) ) {
      BUF << "RPAREN";
      continue;
    }

    // let's play with brackets
    if ( byte == buf_stream.byte( '[' ) ) {
      BUF << "LBRACKET";
      continue;
    }
    if ( byte == buf_stream.byte( ']' ) ) {
      BUF << "RBRACKET";
      continue;
    }

    // let's play with curly braces
    if ( byte == buf_stream.byte( '{' ) ) {
      BUF << "LBRACE";
      continue;
    }
    if ( byte == buf_stream.byte( '}' ) ) {
      BUF << "RBRACE";
      continue;
    }

    // let's play with the comma operator
    if ( byte == buf_stream.byte( ',' ) ) {
      BUF << "COMMA";
      continue;
    }

    // let's play with the colon operator
    if ( byte == buf_stream.byte( ':' ) ) {
      BUF << "COLON";
      continue;
    }

    // let's play with the semicolon operator
    if ( byte == buf_stream.byte( ';' ) ) {
      BUF << "SEMICOLON";
      continue;
    }

    // let's play with the plus operator
    if ( byte == buf_stream.byte( '+' ) ) {
      BUF << "PLUS";
      continue;
    }

    // let's play with the minus operator
    if ( byte == buf_stream.byte( '-' ) ) {
      BUF << "MINUS";
      continue;
    }

    // let's play with the asterisk operator
    if ( byte == buf_stream.byte( '*' ) ) {
      BUF << "ASTERISK";
      continue;
    }

    // let's play with the slash operator
    if ( byte == buf_stream.byte( '/' ) ) {
      BUF << "SLASH";
      continue;
    }

    // let's play with the percent operator
    if ( byte == buf_stream.byte( '%' ) ) {
      BUF << "PERCENT";
      continue;
    }

    // let's play with the ampersand operator
    if ( byte == buf_stream.byte( '&' ) ) {
      BUF << "AMPERSAND";
      continue;
    }

    // let's play with the pipe operator
    if ( byte == buf_stream.byte( '|' ) ) {
      BUF << "PIPE";
      continue;
    }

    // let's play with the caret operator
    if ( byte == buf_stream.byte( '^' ) ) {
      BUF << "CARET";
      continue;
    }

    // let's play with the tilde operator
    if ( byte == buf_stream.byte( '~' ) ) {
      BUF << "TILDE";
      continue;
    }

    // let's play with the exclamation operator
    if ( byte == buf_stream.byte( '!' ) ) {
      BUF << "EXCLAMATION";
      continue;
    }

    // let's play with the question operator
    if ( byte == buf_stream.byte( '?' ) ) {
      BUF << "QUESTION";
      continue;
    }

    // let's play with the at operator
    if ( byte == buf_stream.byte( '@' ) ) {
      BUF << "AT";
      continue;
    }

    // let's play with the hash operator
    if ( byte == buf_stream.byte( '#' ) ) {
      BUF << "HASH";
      continue;
    }

    // let's play with the dollar operator
    if ( byte == buf_stream.byte( '$' ) ) {

      BUF << "DOLLAR";
      continue;
    }

    // let's play with low-dash operator
    if ( byte == buf_stream.byte( '_' ) ) {
      BUF << "_";
      continue;
    }

    // let's play with string single quotes
    if ( byte == buf_stream.byte( '\'' ) ) {

      BUF << "S_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        BUF << static_cast<char>( *str );
      }
      BUF << "S_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }
    // let's play with string double quotes
    if ( byte == buf_stream.byte( '"' ) ) {

      BUF << "D_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        BUF << static_cast<char>( *str );
      }
      BUF << "D_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }

    // let's play with the equal sign operator
    if ( byte == buf_stream.byte( '=' ) ) {
      buf_stream.set_stream_col( x + 1 );

      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '=' ) ) {

        if ( buf_stream.get( y, x + 2 ) == buf_stream.byte( '=' ) ) {
          BUF << "STRICT_EQUAL";
          buf_stream.set_stream_col( x + 3 );
          continue;
        }
        BUF << "LOOSE_EQUAL";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      // it may be an arrow function on the equal game.
      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '>' ) ) {
        BUF << "ARROW";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      BUF << "EQUAL";
    }
  }

  // to use the stream in threads is necessary to create a registry of the streams
  /*std::thread stream_thread_0( [&buf_stream]() {
    while ( auto c = buf_stream.stream() ) {
    auto [ location, byte ] = *c;
    auto [ y, x ] = location;

    // skip newlines
    if ( byte == buf_stream.byte( '\n' ) ) {
      continue;
    }

    // skip whitespace and nullbytes if any
    if ( byte == buf_stream.byte( ' ' ) || byte == buf_stream.byte( '\t' ) || byte == buf_stream.byte( '\r' ) ||
         byte == buf_stream.byte( '\0' ) || byte == buf_stream.byte( '\v' ) || byte == buf_stream.byte( '\f' ) ) {
      continue;
    }

    // let's print everything that is alphanumeric
    if ( std::isalnum( static_cast<char>( byte ) ) ) {
      BUF << static_cast<char>( byte );
    }

    // let's play with the dot operator
    if ( byte == buf_stream.byte( '.' ) ) {
      BUF << "DOT";
      continue;
    }

    // let's play with the parenthesis
    if ( byte == buf_stream.byte( '(' ) ) {
      BUF << "LPAREN";
      continue;
    }
    if ( byte == buf_stream.byte( ')' ) ) {
      BUF << "RPAREN";
      continue;
    }

    // let's play with brackets
    if ( byte == buf_stream.byte( '[' ) ) {
      BUF << "LBRACKET";
      continue;
    }
    if ( byte == buf_stream.byte( ']' ) ) {
      BUF << "RBRACKET";
      continue;
    }

    // let's play with curly braces
    if ( byte == buf_stream.byte( '{' ) ) {
      BUF << "LBRACE";
      continue;
    }
    if ( byte == buf_stream.byte( '}' ) ) {
      BUF << "RBRACE";
      continue;
    }

    // let's play with the comma operator
    if ( byte == buf_stream.byte( ',' ) ) {
      BUF << "COMMA";
      continue;
    }

    // let's play with the colon operator
    if ( byte == buf_stream.byte( ':' ) ) {
      BUF << "COLON";
      continue;
    }

    // let's play with the semicolon operator
    if ( byte == buf_stream.byte( ';' ) ) {
      BUF << "SEMICOLON";
      continue;
    }

    // let's play with the plus operator
    if ( byte == buf_stream.byte( '+' ) ) {
      BUF << "PLUS";
      continue;
    }

    // let's play with the minus operator
    if ( byte == buf_stream.byte( '-' ) ) {
      BUF << "MINUS";
      continue;
    }

    // let's play with the asterisk operator
    if ( byte == buf_stream.byte( '*' ) ) {
      BUF << "ASTERISK";
      continue;
    }

    // let's play with the slash operator
    if ( byte == buf_stream.byte( '/' ) ) {
      BUF << "SLASH";
      continue;
    }

    // let's play with the percent operator
    if ( byte == buf_stream.byte( '%' ) ) {
      BUF << "PERCENT";
      continue;
    }

    // let's play with the ampersand operator
    if ( byte == buf_stream.byte( '&' ) ) {
      BUF << "AMPERSAND";
      continue;
    }

    // let's play with the pipe operator
    if ( byte == buf_stream.byte( '|' ) ) {
      BUF << "PIPE";
      continue;
    }

    // let's play with the caret operator
    if ( byte == buf_stream.byte( '^' ) ) {
      BUF << "CARET";
      continue;
    }

    // let's play with the tilde operator
    if ( byte == buf_stream.byte( '~' ) ) {
      BUF << "TILDE";
      continue;
    }

    // let's play with the exclamation operator
    if ( byte == buf_stream.byte( '!' ) ) {
      BUF << "EXCLAMATION";
      continue;
    }

    // let's play with the question operator
    if ( byte == buf_stream.byte( '?' ) ) {
      BUF << "QUESTION";
      continue;
    }

    // let's play with the at operator
    if ( byte == buf_stream.byte( '@' ) ) {
      BUF << "AT";
      continue;
    }

    // let's play with the hash operator
    if ( byte == buf_stream.byte( '#' ) ) {
      BUF << "HASH";
      continue;
    }

    // let's play with the dollar operator
    if ( byte == buf_stream.byte( '$' ) ) {

      BUF << "DOLLAR";
      continue;
    }

    // let's play with low-dash operator
    if ( byte == buf_stream.byte( '_' ) ) {
      BUF << "_";
      continue;
    }

    // let's play with string single quotes
    if ( byte == buf_stream.byte( '\'' ) ) {

      BUF << "S_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        BUF << static_cast<char>( *str );
      }
      BUF << "S_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }
    // let's play with string double quotes
    if ( byte == buf_stream.byte( '"' ) ) {

      BUF << "D_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        BUF << static_cast<char>( *str );
      }
      BUF << "D_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }

    // let's play with the equal sign operator
    if ( byte == buf_stream.byte( '=' ) ) {
      buf_stream.set_stream_col( x + 1 );

      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '=' ) ) {

        if ( buf_stream.get( y, x + 2 ) == buf_stream.byte( '=' ) ) {
          BUF << "STRICT_EQUAL";
          buf_stream.set_stream_col( x + 3 );
          continue;
        }
        BUF << "LOOSE_EQUAL";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      // it may be an arrow function on the equal game.
      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '>' ) ) {
        BUF << "ARROW";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      BUF << "EQUAL";
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
    if ( byte == buf_stream.byte( ' ' ) || byte == buf_stream.byte( '\t' ) || byte == buf_stream.byte( '\r' ) ||
         byte == buf_stream.byte( '\0' ) || byte == buf_stream.byte( '\v' ) || byte == buf_stream.byte( '\f' ) ) {
      continue;
    }

    // let's print everything that is alphanumeric
    if ( std::isalnum( static_cast<char>( byte ) ) ) {
      BUF << static_cast<char>( byte );
    }

    // let's play with the dot operator
    if ( byte == buf_stream.byte( '.' ) ) {
      BUF << "DOT";
      continue;
    }

    // let's play with the parenthesis
    if ( byte == buf_stream.byte( '(' ) ) {
      BUF << "LPAREN";
      continue;
    }
    if ( byte == buf_stream.byte( ')' ) ) {
      BUF << "RPAREN";
      continue;
    }

    // let's play with brackets
    if ( byte == buf_stream.byte( '[' ) ) {
      BUF << "LBRACKET";
      continue;
    }
    if ( byte == buf_stream.byte( ']' ) ) {
      BUF << "RBRACKET";
      continue;
    }

    // let's play with curly braces
    if ( byte == buf_stream.byte( '{' ) ) {
      BUF << "LBRACE";
      continue;
    }
    if ( byte == buf_stream.byte( '}' ) ) {
      BUF << "RBRACE";
      continue;
    }

    // let's play with the comma operator
    if ( byte == buf_stream.byte( ',' ) ) {
      BUF << "COMMA";
      continue;
    }

    // let's play with the colon operator
    if ( byte == buf_stream.byte( ':' ) ) {
      BUF << "COLON";
      continue;
    }

    // let's play with the semicolon operator
    if ( byte == buf_stream.byte( ';' ) ) {
      BUF << "SEMICOLON";
      continue;
    }

    // let's play with the plus operator
    if ( byte == buf_stream.byte( '+' ) ) {
      BUF << "PLUS";
      continue;
    }

    // let's play with the minus operator
    if ( byte == buf_stream.byte( '-' ) ) {
      BUF << "MINUS";
      continue;
    }

    // let's play with the asterisk operator
    if ( byte == buf_stream.byte( '*' ) ) {
      BUF << "ASTERISK";
      continue;
    }

    // let's play with the slash operator
    if ( byte == buf_stream.byte( '/' ) ) {
      BUF << "SLASH";
      continue;
    }

    // let's play with the percent operator
    if ( byte == buf_stream.byte( '%' ) ) {
      BUF << "PERCENT";
      continue;
    }

    // let's play with the ampersand operator
    if ( byte == buf_stream.byte( '&' ) ) {
      BUF << "AMPERSAND";
      continue;
    }

    // let's play with the pipe operator
    if ( byte == buf_stream.byte( '|' ) ) {
      BUF << "PIPE";
      continue;
    }

    // let's play with the caret operator
    if ( byte == buf_stream.byte( '^' ) ) {
      BUF << "CARET";
      continue;
    }

    // let's play with the tilde operator
    if ( byte == buf_stream.byte( '~' ) ) {
      BUF << "TILDE";
      continue;
    }

    // let's play with the exclamation operator
    if ( byte == buf_stream.byte( '!' ) ) {
      BUF << "EXCLAMATION";
      continue;
    }

    // let's play with the question operator
    if ( byte == buf_stream.byte( '?' ) ) {
      BUF << "QUESTION";
      continue;
    }

    // let's play with the at operator
    if ( byte == buf_stream.byte( '@' ) ) {
      BUF << "AT";
      continue;
    }

    // let's play with the hash operator
    if ( byte == buf_stream.byte( '#' ) ) {
      BUF << "HASH";
      continue;
    }

    // let's play with the dollar operator
    if ( byte == buf_stream.byte( '$' ) ) {

      BUF << "DOLLAR";
      continue;
    }

    // let's play with low-dash operator
    if ( byte == buf_stream.byte( '_' ) ) {
      BUF << "_";
      continue;
    }

    // let's play with string single quotes
    if ( byte == buf_stream.byte( '\'' ) ) {

      BUF << "S_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        BUF << static_cast<char>( *str );
      }
      BUF << "S_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }
    // let's play with string double quotes
    if ( byte == buf_stream.byte( '"' ) ) {

      BUF << "D_QUOTE";
      x = x +1;

      while ( auto str = buf_stream.stream( y, x++, byte ) ) {
        buf_stream.set_stream_col( x );
        BUF << static_cast<char>( *str );
      }
      BUF << "D_QUOTE";
      buf_stream.set_stream_col( x );
      continue;
    }

    // let's play with the equal sign operator
    if ( byte == buf_stream.byte( '=' ) ) {
      buf_stream.set_stream_col( x + 1 );

      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '=' ) ) {

        if ( buf_stream.get( y, x + 2 ) == buf_stream.byte( '=' ) ) {
          BUF << "STRICT_EQUAL";
          buf_stream.set_stream_col( x + 3 );
          continue;
        }
        BUF << "LOOSE_EQUAL";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      // it may be an arrow function on the equal game.
      if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '>' ) ) {
        BUF << "ARROW";
        buf_stream.set_stream_col( x + 2 );
        continue;
      }
      BUF << "EQUAL";
    }
  }
  });

  stream_thread_0.join();
  stream_thread_1.join();*/

  log::flush( "buf" );
  log::flush( "buffer" );
  return 0;
}
