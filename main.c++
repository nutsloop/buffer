#include "buffer.h++"

int main() {

  constexpr nutsloop::u8 lines = 3;
  constexpr nutsloop::u16 bytes_per_line = 20;

  // creating a buffer with a registry
  nutsloop::buffer buffer( true );
  buffer.allocate_into( "main_buffer" );
  buffer.allocate_into( "another_buffer" );
  buffer.allocate_into( "yet_another_buffer with allocation", lines, bytes_per_line );


  return 0;
}
