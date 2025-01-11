#include <iostream>
#include <thread>

#include "buffer.h++"

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

int main() {

  constexpr nutsloop::u8 lines = 3;
  constexpr nutsloop::u16 bytes_per_line = 20;

  // creating a buffer with a registry
  nutsloop::buffer buffer( true );
  buffer.allocate_into( "main_buffer" );
  buffer.allocate_into( "another_buffer" );
  buffer.allocate_into( "yet_another_buffer with allocation", lines, bytes_per_line );

  nutsloop::buffer buf_fromfile;
  buf_fromfile.read( "/Users/simonedelpopolo/.zsh_history" );

  nutsloop::buffer buf_fromfd;
  const auto path = "/Users/simonedelpopolo/.zsh_history";
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

  buf_fromfd.read( std::filesystem::path{path}, fd_, file_size );

  BUFFER << std::format("buf_fromfd before removing line 4: size -> {}", buf_fromfd.size()) << '\n';

  buf_fromfd >> buf_fromfd.delete_at( 3 );

  BUFFER << std::format("buf_fromfd after removing line 4: size -> {}", buf_fromfd.size()) << '\n';
  BUFFER << std::format("buf_fromfd line 4 size -> {}", buf_fromfd.size(3)) << '\n';
  BUFFER << buf_fromfd.to_string( 3 ) << '\n';

  buf_fromfd >> buf_fromfd.delete_at( 3, 2 );

  BUFFER << std::format("buf_fromfd line 4 size -> {}", buf_fromfd.size(3)) << '\n';
  BUFFER << buf_fromfd.to_string( 3 ) << '\n';

  buf_fromfd << std::make_tuple( 3, buf_fromfd.size( 3 ), buf_fromfd.byte('!') );
  buf_fromfd << std::make_tuple( 3, buf_fromfd.size( 3 ), buf_fromfd.byte('!') );

  BUFFER << std::format("buf_fromfd line 4 size -> {}", buf_fromfd.size(3)) << '\n';
  BUFFER << buf_fromfd.to_string( 3 ) << '\n';

  // Simulate uploading byte by byte
  while (auto byte_opt = buf_fromfd.stream()) {
    nutsloop::nuts_byte_t b = *byte_opt;
    std::cout << static_cast<char>(b) << std::flush;
    //std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
  }

  while (auto byte_opt = buf_fromfd.stream(1057)) {
    nutsloop::nuts_byte_t b = *byte_opt;
    std::cout << static_cast<char>(b) << std::flush;
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
  }

  ::close(fd_);
  return 0;
}
