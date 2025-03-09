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

  const std::filesystem::path path = std::getenv( "JSX_FILE_BUFFER" );
  if ( path.empty() ) {
    std::cerr << "JSX_FILE_BUFFER environment variable not set" << std::endl;
    exit( 1 );
  }

  nutsloop::buffer buf_fromfile( path );

  std::cout << std::format( "buf_fromfile before removing line 2: size -> {}", buf_fromfile.size() ) << '\n';

  // removing line in the buffer using overload operator '>>'
  buf_fromfile >> std::make_tuple( 1, std::nullopt );

  std::cout << std::format( "buf_fromfile after removing line 2: size -> {}", buf_fromfile.size() ) << '\n';
  std::cout << std::format( "buf_fromfile line 3 size -> {}", buf_fromfile.size( 2 ) ) << '\n';
  std::cout << buf_fromfile.to_string( 2 ) << '\n';

  // removing bytes in the buffer using overload operator '>>'
  buf_fromfile >> std::make_tuple( 2, 29 );
  buf_fromfile >> std::make_tuple( 2, 29 );

  std::cout << "buf_fromfile line 3 removed two chars at column 29" << '\n';
  std::cout << std::format( "buf_fromfile line 3 size -> {}", buf_fromfile.size( 2 ) ) << '\n';
  std::cout << buf_fromfile.to_string( 2 ) << '\n';

  // inserting bytes in the buffer using overload operator '<<'
  buf_fromfile << std::make_tuple( 2, 29, buf_fromfile.byte( "!" ) );
  buf_fromfile << std::make_tuple( 2, 29, buf_fromfile.byte( "!" ) );

  std::cout << "buf_fromfile line 3 inserted two chars at column 29" << '\n';
  std::cout << std::format( "buf_fromfile line 3 size -> {}", buf_fromfile.size( 2 ) ) << '\n';
  std::cout << buf_fromfile.to_string( 2 ) << '\n';

  // Simulate uploading byte by byte
  auto stream_byte = buf_fromfile.stream();
  while ( auto byte_opt = stream_byte.next() ) {
    auto [ loc_, byte ] = *byte_opt;
    std::cout << buf_fromfile.byte( byte ) << std::flush;
    // std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
  }

  auto stream_line = buf_fromfile.stream();
  while ( auto byte_opt = stream_line.next( static_cast<std::size_t>(2) )) {
    auto [ _loc, byte ] = *byte_opt;
    std::cout << buf_fromfile.byte( byte ) << std::flush;
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
  }

  return 0;
}
