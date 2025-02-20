#include "buffer.h++"

#include <iostream>
#include <thread>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

using namespace nutsloop;
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

int main() {

  constexpr nutsloop::u8 lines = 3;
  constexpr nutsloop::u16 bytes_per_line = 20;
  const auto path = std::getenv( "JSX_FILE_BUFFER" );
  if ( path == nullptr ) {
    std::cerr << "JSX_FILE_BUFFER environment variable not set" << std::endl;
    exit( 1 );
  }

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
  auto stream_byte = buf_fromfd.make_stream();
  while ( auto byte_opt = stream_byte.next() ) {
    auto [ loc_, byte ] = *byte_opt;
    std::cout << static_cast<char>( byte ) << std::flush;
    // std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
  }

  auto stream_line = buf_fromfd.make_stream();
  while ( auto byte_opt = stream_line.next( static_cast<std::size_t>(2) )) {
    auto [ _loc, byte ] = *byte_opt;
    std::cout << static_cast<char>( byte ) << std::flush;
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
  }

  ::close( fd_ );

  return 0;
}
