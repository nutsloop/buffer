#include "buffer.h++"

#include <iostream>
#include <thread>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

int main() {

  constexpr nutsloop::u8 lines = 3;
  constexpr nutsloop::u16 bytes_per_line = 20;

  nutsloop::buffer buffer_empty;
  buffer_empty.allocate( nutsloop::u8{ 24 }, nutsloop::u16{ 100 } );

  // creating a buffer with a registry
  nutsloop::buffer buf( true );
  buf.allocate_into( "main_buffer" );
  buf.allocate_into( "another_buffer" );
  buf.allocate_into( "yet_another_buffer with allocation", lines, bytes_per_line );

  nutsloop::buffer buf_fromfile;
  buf_fromfile.read( "/Volumes/a-ssd/CODE/nutsloop/jsx/index.jsx" );

  nutsloop::buffer buf_fromfd;
  const auto path = "/Volumes/a-ssd/CODE/nutsloop/jsx/index.jsx";
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

  BUFFER << std::format( "buf_fromfd before removing line 4: size -> {}", buf_fromfd.size() ) << '\n';

  buf_fromfd >> buf_fromfd.delete_at( 3 );

  BUFFER << std::format( "buf_fromfd after removing line 4: size -> {}", buf_fromfd.size() ) << '\n';
  BUFFER << std::format( "buf_fromfd line 4 size -> {}", buf_fromfd.size( 3 ) ) << '\n';
  BUFFER << buf_fromfd.to_string( 3 ) << '\n';

  buf_fromfd >> buf_fromfd.delete_at( 3, 2 );

  BUFFER << std::format( "buf_fromfd line 4 size -> {}", buf_fromfd.size( 3 ) ) << '\n';
  BUFFER << buf_fromfd.to_string( 3 ) << '\n';

  buf_fromfd << std::make_tuple( 3, buf_fromfd.size( 3 ), buf_fromfd.byte( '!' ) );
  buf_fromfd << std::make_tuple( 3, buf_fromfd.size( 3 ), buf_fromfd.byte( '!' ) );

  BUFFER << std::format( "buf_fromfd line 4 size -> {}", buf_fromfd.size( 3 ) ) << '\n';
  BUFFER << buf_fromfd.to_string( 3 ) << '\n';

  // Simulate uploading byte by byte
  while ( auto byte_opt = buf_fromfd.stream() ) {
    nutsloop::nuts_byte_t b = *byte_opt;
    std::cout << static_cast<char>( b ) << std::flush;
    // std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
  }

  while ( auto byte_opt = buf_fromfd.stream( 1057 ) ) {
    nutsloop::nuts_byte_t b = *byte_opt;
    std::cout << static_cast<char>( b ) << std::flush;
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
  }

  ::close( fd_ );

  // MARK: work with stream methods
  // small tokenizer base on jsx file
  /**

```javascript
  function component(props) {

  this.handleButtonClick = () => {
    console.log('button clicked');
  }

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
  buffer buf_stream;
  buf_stream.read( "/Volumes/a-ssd/CODE/nutsloop/jsx/index.jsx" );
  const nutsloop::log_settings_t settings( "buf", "buf.log", true, std::nullopt, std::nullopt );

  log::set( settings );

  size_t i = 0;
  size_t x = 0;
  size_t y = 0;

  BUF << "buf_stream.size() -> " << buf_stream.size() << '\n';

  while ( auto c = buf_stream.stream() ) {

    if ( *c == buf_stream.byte( '\n' ) ) {
      x = 0;
      y++;
      continue;
    }

    if ( *c == buf_stream.byte( ' ' ) || *c == buf_stream.byte( '\t' ) || *c == buf_stream.byte( '\r' ) ||
         *c == buf_stream.byte( '\0' ) || *c == buf_stream.byte( '\v' ) || *c == buf_stream.byte( '\f' ) ) {
      x++;
      continue;
    }

    if ( *c != buf_stream.byte( '=' ) ) {
      BUF << static_cast<char>( *c );
    }

    if ( *c == buf_stream.byte( '=' ) ) {
      x++;
      buf_stream.set_stream_col( x );

      if ( buf_stream.get( y, x ) == buf_stream.byte( '=' ) ) {

        if ( buf_stream.get( y, x + 1 ) == buf_stream.byte( '=' ) ) {
          BUF << "STRICT_EQUAL";
          x++;
          buf_stream.set_stream_col( x + 1 );
          continue;
        }
        x++;
        BUF << "LOOSE_EQUAL";
        buf_stream.set_stream_col( x + 1 );
        continue;
      }
      if ( buf_stream.get( y, x ) == buf_stream.byte( '>' ) ) {
        BUF << "ARROW";
        x++;
        buf_stream.set_stream_col( x + 1 );
        continue;
      }
      BUF << "EQUAL";
      continue;
    }

    x++;
  }

  log::flush( "buf" );
  log::flush( "buffer" );
  return 0;
}
