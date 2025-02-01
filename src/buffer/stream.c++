#include "buffer.h++"

namespace nutsloop {

std::optional<nuts_byte_t> buffer::stream() {
  {
    // MARK: (buffer) MUTEX_LOCK
    std::unique_lock lock( mtx_ );
    // If we've reached the end of the buffer, return nullopt to end the stream.
    if ( stream_line_ >= size() ){

      stream_line_.store( 0 );                // Reset line index
      stream_col_.store( 0 );                 // Reset column index
      stream_byte_.store(nuts_byte_t{0x00});  // Reset byte

      return std::nullopt; // End of stream
    }

    // Unlined the current line
    auto& line =  nuts_buffer_[ stream_line_ ];

    // If we've finished the current line, return '\n'
    if ( stream_col_ >= line.size() ) {
      stream_line_.fetch_add( 1 );// Move to the next line
      stream_col_.store( 0 ); // Reset column index
      return nuts_byte_t{ '\n' }; // Return newline
    }

    return line[ stream_col_.fetch_add( 1 ) ]; // Return next byte
  }
}

// TODO: must be adapted to modifiers
std::optional<nuts_byte_t> buffer::stream( const size_t line ) const {
  // Check if the line index is valid
  if ( line >= nuts_buffer_.size() )
    return std::nullopt; // No such line

  static size_t col_idx_ = 0; // Column index, resettable per stream call

  const auto& target_line = nuts_buffer_[ line ];

  // End of line
  if ( col_idx_ >= target_line.size() ) {
    col_idx_ = 0; // Reset column index for next usage
    return std::nullopt; // Signal end of the line
  }

  // Return the next byte in the line
  return target_line[ col_idx_++ ];
}

// TODO: handle exceeding sizes
std::size_t buffer::set_stream_line( const std::size_t line_n ) {
  {
    // MARK: (buffer) MUTEX_LOCK
    std::unique_lock lock( mtx_ );
    stream_col_.store( 0 );
    if ( line_n > size() ) {
      throw std::runtime_error( "buffer::set_stream_line(): line index exceeds buffer size" );
    }
    return stream_line_.exchange( line_n );
  }
}

// TODO: handle exceeding sizes
std::size_t buffer::set_stream_col( const std::size_t col_n ) {
  {
    // MARK: (buffer) MUTEX_LOCK
    std::unique_lock lock( mtx_ );
    if ( col_n > size(stream_line_) ) {
      throw std::runtime_error( "buffer::set_stream_col(): column index exceeds buffer size" );
    }
    return stream_col_.exchange( col_n );
  }
}

std::size_t buffer::end_stream() {
  {
    // MARK: (buffer) MUTEX_LOCK
    std::unique_lock lock( mtx_ );
    return stream_line_.exchange( size() );
  }
}

}
