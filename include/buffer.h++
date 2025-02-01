#pragma once

// MARK: (buffer) buffer debug pre-processor
#ifndef DEBUG_BUFFER
// HINT: default value if not defined elsewhere
// HINT: set to false also to find out what include c++ std headers are needed.
#define DEBUG_BUFFER true
#endif

#if DEBUG_BUFFER == true

#warning "DEBUG_BUFFER is enabled"

#include "log.h++"

constexpr bool DEBUG_BUFFER_CONST = true;

#define BUFFER log::stream( "buffer", __FILE__, __LINE__, nutsloop::Level::INFO )
#define BUFFER_WARN log::stream( "buffer", __FILE__, __LINE__, nutsloop::Level::WARN )
#define BUFFER_ERROR log::stream( "buffer", __FILE__, __LINE__, nutsloop::Level::ERROR )

#else

constexpr bool DEBUG_BUFFER_CONST = false;

// Mock macros to provide `<<` compatibility
#include <sstream>

#define BUFFER std::ostream( nullptr ) // No-op stream
#define BUFFER_WARN std::ostream( nullptr ) // No-op stream
#define BUFFER_ERROR std::ostream( nullptr ) // No-op stream

#endif

#include "types.h++"

#include <atomic>
#include <shared_mutex>

namespace nutsloop {

using namespace nbuffer::types;

class buffer {

public:
  /**
   * Default constructor for the buffer class.
   * Initializes an empty buffer instance without allocating any resources.
   *
   * @return A newly constructed buffer object with default state.
   */
  buffer();

  /**
   * Constructs a buffer object, optionally initializing it with a registry.
   * If the `has_registry` parameter is true, a new registry is created.
   *
   * @param has_registry Indicates whether the buffer should initialize with a registry.
   *                     If set to true, a registry object will be allocated and configured.
   * @return An instance of the buffer class initialized based on the provided flag.
   */
  explicit buffer( bool has_registry );

  // these are when reading from a file and optionally inserting into registry
  // HINT: not implemented yet
  buffer( const std::filesystem::path& file_path, std::optional<std::string&> ident );
  // HINT: not implemented yet
  buffer( const int& fd, const off_t& file_size, std::optional<std::string&> ident );

  /**
   * @brief Constructs a `buffer` object with specified allocation size and bytes per line.
   *
   * @tparam A Type of the allocation size parameter. Must satisfy the `ValidIntegerTypes_C` concept.
   * @tparam B Type of the bytes per line parameter. Must satisfy the `ValidIntegerTypes_C` concept.
   *
   * @param allocation An optional parameter specifying the allocation size for the buffer. Defaults to 1.
   * @param bytes_per_line An optional parameter specifying the number of bytes per line. Defaults to 1.
   * @requires The types A and B must meet the requirements of the `ValidIntegerTypes_C` concept.
   *
   * @throws std::invalid_argument If the buffer has an existing registry, if allocation exceeds the maximum value of
   * type A, or if bytes per line exceed the maximum value of type B.
   */
  template <typename A = u8, typename B = u8>
  explicit buffer( A allocation = 1, B bytes_per_line = 1 )
    requires ValidIntegerTypes<A> && ValidIntegerTypes<B>;

  /**
   * Allocates memory for the buffer based on the specified allocation size and bytes per line.
   * Performs validation to ensure the parameters meet constraints and preconditions.
   * Throws exceptions if the buffer is already allocated or if the parameters are invalid.
   *
   * @param allocation The number of allocation units to reserve for the buffer. Must be of an unsigned type.
   * @param bytes_per_line The number of bytes attributed to each line of the buffer. Must be of an unsigned type.
   *
   * @throws std::invalid_argument If the buffer has a registry, or if the allocation size or bytes per line exceed
   * their respective maximum values.
   * @throws std::logic_error If the buffer is already allocated.
   */
  template <typename A = u8, typename B = u8>
  void allocate( A allocation = 1, B bytes_per_line = 1 )
    requires ValidIntegerTypes<A> && ValidIntegerTypes<B>;

  /**
   * Allocates memory for a buffer and manages its metadata registry. This method initializes
   * the buffer and inserts the related metadata into the registry. It validates inputs and
   * ensures correct states before allocation.
   *
   * @param ident The identifier associated with the allocated buffer for use in metadata registry.
   * @param allocation The number of elements to allocate in the buffer. Must be an unsigned type.
   * @param bytes_per_line The number of bytes per line in the buffer. Must be an unsigned type.
   *
   * @throws std::invalid_argument Thrown if the registry is not properly initialized, or if
   *         the allocation size or bytes per line exceed the allowed maximum values.
   * @throws std::logic_error Thrown if the buffer is already allocated, preventing duplicate
   *         allocations.
   */
  template <typename A = u8, typename B = u8>
  void allocate_into( std::string ident, A allocation = 1, B bytes_per_line = 1 )
    requires ValidIntegerTypes<A> && ValidIntegerTypes<B>;

  // MARK: (buffer) buffer registry methods
  // HINT: not implemented yet
  void registry();
  // HINT: not implemented yet
  void registry( std::string& ident );

  /**
   * Reads the contents of a file specified by the given file path and processes its data into the buffer.
   *
   * @param file_path The path to the file to be read. It must point to a valid, accessible file on the filesystem.
   *
   * @throws std::invalid_argument If the file does not exist at the specified path.
   * @throws std::runtime_error If the file cannot be opened for reading.
   */
  void read( const std::filesystem::path& file_path );
  /**
   * Reads the content of a file specified by the given file path and file descriptor, processes
   * it in chunks, and populates the buffer with the read data. Metadata about the read operation
   * is also recorded.
   *
   * @param file_path The file path of the file to be read. Used for metadata recording.
   * @param fd The file descriptor associated with the file being read.
   * @param file_size The total size of the file in bytes.
   *
   * @throws std::runtime_error If the `::read` [<fcntl.h>] function return `bytes_read<0`
   */
  void read( const std::filesystem::path& file_path, const int& fd, const off_t& file_size );

  // HINT: not implemented yet
  void read_into( std::string ident, const std::filesystem::path& file_path );
  // HINT: not implemented yet
  void read_into( std::string ident, const std::filesystem::path& file_path, const int& fd, const off_t& file_size );

  // HINT: not implemented yet
  void write( const std::filesystem::path& file_path );
  // HINT: not implemented yet
  void write( const int& fd );

private:
  // MARK: (buffer) mutex methods and fields
  std::shared_mutex mtx_;

  /**
   * Generates the hexadecimal address of the internal nuts_buffer_ memory location.
   * It uses the address of the internal buffer data and returns it in a formatted string.
   *
   * @return A string representation of the memory address of nuts_buffer_ in hexadecimal format.
   */
  std::string addr_hex_();
  nuts_buffer_t nuts_buffer_;
  nuts_buffer_unlined_t nuts_buffer_unlined_;
  nuts_byte_t nuts_byte_{ 0x00 };

  /**
   * Retrieves the current allocation status of the buffer.
   * The method checks and returns whether the buffer is currently allocated.
   * It uses a shared lock to ensure thread-safe access to the allocation status.
   *
   * @return A boolean value indicating whether the buffer is allocated.
   */
  bool get_allocated_();
  /**
   * Marks the buffer as allocated by updating its internal state.
   * Ensures thread-safe operation using a mutex and provides debug output
   * if debugging is enabled.
   *
   * This method transitions the `allocated_` flag to `true` while preserving the
   * previous state for potential debug logging purposes.
   */
  void set_allocated_();
  /**
   * Marks the buffer as not allocated by updating the allocated_ flag to false.
   * If debugging is enabled, logs the previous and updated states of the allocated_ flag.
   *
   * This method also ensures thread safety by using a shared lock for accessing
   * and logging the state of the allocated_ flag.
   */
  void unset_allocated_();
  std::atomic<bool> allocated_{ false };

  // MARK: (buffer) metadata methods and fields
  /**
   * Inserts metadata into the buffer. This metadata includes memory address,
   * an optional filename, and an optional registry identifier.
   * Updates the internal metadata structure with the provided values.
   *
   * @param mem_addr The memory address of type nuts_buffer_mem_addr_t.
   * @param filename An optional file name of type nuts_buffer_from_file_t,
   *        representing the source file information. Pass std::nullopt if not applicable.
   * @param ident An optional registry identifier of type nuts_buffer_registry_identifier_t,
   *        used for tracking or identifying associated metadata. Pass std::nullopt if not applicable.
   */
  void insert_metadata_( const nuts_buffer_mem_addr_t& mem_addr, const nuts_buffer_from_file_t& filename,
                         const nuts_buffer_registry_identifier_t& ident );
  nuts_buffer_metadata_t metadata_{};

  /**
   * Retrieves the current state of the read flag.
   *
   * This method acquires a shared lock for thread-safe access to the
   * internal read flag and provides its current value. If debugging is
   * enabled, it also logs the operation and the state of the read flag.
   *
   * @return The current state of the read flag (true if active, false otherwise).
   */
  bool get_read_();
  /**
   * Marks the buffer as read by updating the internal state.
   * This method updates the read status of the buffer to indicate
   * that it has been accessed for reading. Thread-safe operation
   * using atomic exchange to ensure consistency in multithreaded
   * environments. When in debug mode, log the state change.
   */
  void set_read_();
  std::atomic<bool> read_{ false };

  // MARK: (buffer) registry methods and fields
  /**
   * Checks if the buffer has been initialized with a registry.
   *
   * This method provides a thread-safe mechanism to retrieve the state of
   * the internal registry flag using atomic operations. It also logs
   * its activity when debugging is enabled.
   *
   * @return The current state of the `has_registry_` flag, where `true`
   *         indicates the presence of a registry and `false` indicates otherwise.
   */
  bool get_has_registry_();
  /**
   * Sets the `has_registry_` flag to `true` in a thread-safe manner.
   * This method updates the internal state of the flag using an atomic
   * operation to ensure consistency in a multithreaded environment. It can
   * optionally log the state transitions if debugging is enabled.
   */
  void set_has_registry_();
  std::atomic<bool> has_registry_{ false };
  std::unique_ptr<nuts_buffer_registry_t> registry_{ nullptr };

  // MARK: (buffer) buffer debug methods and fields
  /**
   * Checks if debugging is activated for the buffer object.
   *
   * If the debug mode is not already activated, this method will activate it by
   * invoking the `debug_activate_()` method.
   */
  void debug_is_activated_();
  /**
   * Activates debug logging for the buffer class.
   * This function enables detailed debug output for the buffer instance by setting
   * up logging configurations and activating the debug flag. If debug logging
   * was already active, it logs the transition status.
   *
   * The function configures log settings, enabling logs with specific identifiers
   * and output filenames. Information about the transition of the debug status
   * and the applied log settings are written to the debug buffer.
   */
  void debug_activate_();
  std::atomic<bool> debug_activated_{ false };
// ONGOING: experimentation.
public:
  // Overload << for insertion
  buffer& operator<<( const std::tuple<size_t, size_t, nuts_byte_t>& insertion ) {
    const size_t line = std::get<0>( insertion ); // Line index
    const auto col = static_cast<std::ptrdiff_t>( std::get<1>( insertion ) ); // Column index
    const nuts_byte_t value = std::get<2>( insertion ); // Byte value to insert

    if ( line >= nuts_buffer_.size() )
      throw std::out_of_range( "Line index out of range" );

    auto& target_line = nuts_buffer_[ line ];
    if ( col > target_line.size() ) // Allow appending at the end
      throw std::out_of_range( "Column index out of range" );

    target_line.insert( target_line.begin() + col, value );
    return *this;
  }

  // Overload >> for deletion
  buffer& operator>>( const std::tuple<size_t, std::optional<size_t>>& deletion ) {
    const auto line = static_cast<std::ptrdiff_t>( std::get<0>( deletion ) ); // Line index

    BUFFER << "INTO OPERATOR >>" << '\n';
    if ( line >= nuts_buffer_.size() )
      throw std::out_of_range( "Line index out of range" );

    if ( std::get<1>( deletion ).has_value() ) {
      // Column index provided: delete a specific character
      const auto col = static_cast<std::ptrdiff_t>( std::get<1>( deletion ).value() );
      auto& target_line = nuts_buffer_[ line ];

      if ( col >= target_line.size() )
        throw std::out_of_range( "Column index out of range" );

      target_line.erase( target_line.begin() + col );
    }
    else {
      // No column index: delete the entire line
      nuts_buffer_.erase( nuts_buffer_.begin() + line );
    }

    return *this;
  }

  std::tuple<size_t, std::optional<size_t>> delete_at( size_t line, const std::optional<size_t> col = std::nullopt ) {
    return { line, col };
  }

  std::size_t size() const { return nuts_buffer_.size(); }

  std::size_t size( const std::size_t line ) const { return nuts_buffer_[ line ].size(); }

  nuts_byte_t byte( char c ) { return nuts_byte_ = static_cast<std::byte>( c ); }

  std::string to_string() const {
    std::string buffer_string;
    for ( const auto& line : nuts_buffer_ ) {
      for ( const auto& byte : line ) {
        buffer_string.push_back( static_cast<char>( byte ) );
      }
      buffer_string.push_back( '\n' );
    }
    return buffer_string;
  }

  std::string to_string( const std::size_t line ) const {

    std::string buffer_string;
    for ( const auto& byte : nuts_buffer_[ line ] ) {
      buffer_string.push_back( static_cast<char>( byte ) );
    }
    return buffer_string;
  }

  // Fetch the next byte in the stream
  std::optional<nuts_byte_t> stream() const {
    static size_t line_idx_ = 0;
    static size_t col_idx_ = 0;
    if ( line_idx_ >= size() )
      return std::nullopt; // End of stream

    const auto& line = nuts_buffer_[ line_idx_ ];
    // If we've finished the current line, return '\n'
    if ( col_idx_ == line.size() ) {
      ++line_idx_; // Move to the next line
      col_idx_ = 0; // Reset column index
      return nuts_byte_t{ '\n' }; // Return newline
    }

    return line[ col_idx_++ ];
  }

  std::optional<nuts_byte_t> stream( const size_t line ) const {
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
};

// MARK: (buffer) templates definition

template <typename A, typename B>
buffer::buffer( A allocation, B bytes_per_line )
  requires ValidIntegerTypes<A> && ValidIntegerTypes<B>
{

  // Ensure both types are unsigned
  static_assert( std::is_unsigned_v<A>, "A must be an unsigned type" );
  static_assert( std::is_unsigned_v<B>, "B must be an unsigned type" );

  // Check allocation size validity
  constexpr A a_max_value = std::numeric_limits<A>::max();
  if ( allocation > a_max_value ) {
    throw std::invalid_argument( "Allocation size exceeds the maximum value of A" );
  }

  // Check bytes per line validity
  constexpr B b_max_value = std::numeric_limits<B>::max();
  if ( bytes_per_line > b_max_value ) {
    throw std::invalid_argument( "Bytes per line size exceeds the maximum value of B" );
  }

  if ( DEBUG_BUFFER_CONST ) {
    { // MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      this->debug_is_activated_();
      BUFFER << std::format( "buffer::buffer(allocation[{}], bytes_per_line[{}]) called ⇣", allocation, bytes_per_line )
             << '\n';
    }
  }

  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock( mtx_ );
    nuts_buffer_unlined_ = nuts_buffer_unlined_t( bytes_per_line, nuts_byte_ );
    nuts_buffer_ = nuts_buffer_t( allocation, nuts_buffer_unlined_ );
    insert_metadata_( addr_hex_(), std::nullopt, std::nullopt );
  }
}

template <typename A, typename B>
void buffer::allocate( A allocation, B bytes_per_line )
  requires ValidIntegerTypes<A> && ValidIntegerTypes<B>
{

  if ( get_has_registry_() ) {
    throw std::invalid_argument( "this buffer has a registry. use buffer::allocate_into() instead" );
  }

  // Ensure both types are unsigned
  static_assert( std::is_unsigned_v<A>, "A must be an unsigned type" );
  static_assert( std::is_unsigned_v<B>, "B must be an unsigned type" );

  // Check allocation size validity
  constexpr A a_max_value = std::numeric_limits<A>::max();
  if ( allocation > a_max_value ) {
    throw std::invalid_argument( "Allocation size exceeds the maximum value of A" );
  }

  // Check bytes per line validity
  constexpr B b_max_value = std::numeric_limits<B>::max();
  if ( bytes_per_line > b_max_value ) {
    throw std::invalid_argument( "Bytes per line size exceeds the maximum value of B" );
  }

  if ( DEBUG_BUFFER_CONST ) {
    { // MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      this->debug_is_activated_();
      BUFFER << std::format( "buffer::allocate(allocation[{}], bytes_per_line[{}]) called ⇣", allocation,
                             bytes_per_line )
             << '\n';
    }
  }

  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock( mtx_ );

    // Check if the buffer is already allocated
    if ( !nuts_buffer_.empty() ) {
      if ( DEBUG_BUFFER_CONST ) {
        BUFFER << "  but the buffer is already allocated. it will throw a logic::error" << '\n';
      }
      throw std::logic_error( "Buffer is already allocated." );
    }

    nuts_buffer_unlined_ = nuts_buffer_unlined_t( bytes_per_line, nuts_byte_ );
    nuts_buffer_ = nuts_buffer_t( allocation, nuts_buffer_unlined_ );
    insert_metadata_( addr_hex_(), std::nullopt, std::nullopt );
    set_allocated_();
  }
}

template <typename A, typename B>
void buffer::allocate_into( std::string ident, A allocation, B bytes_per_line )
  requires ValidIntegerTypes<A> && ValidIntegerTypes<B>
{

  if ( !get_has_registry_() ) {
    throw std::invalid_argument( "registry_ is false. use buffer::buffer( true ) to initialize the registry" );
  }

  if ( registry_ == nullptr ) {
    throw std::invalid_argument( "registry_ is nullptr. use buffer::buffer( true ) to initialize the registry" );
  }

  // Ensure both types are unsigned
  static_assert( std::is_unsigned_v<A>, "A must be an unsigned type" );
  static_assert( std::is_unsigned_v<B>, "B must be an unsigned type" );

  // Check allocation size validity
  constexpr A a_max_value = std::numeric_limits<A>::max();
  if ( allocation > a_max_value ) {
    throw std::invalid_argument( "Allocation size exceeds the maximum value of A" );
  }

  // Check bytes per line validity
  constexpr B b_max_value = std::numeric_limits<B>::max();
  if ( bytes_per_line > b_max_value ) {
    throw std::invalid_argument( "Bytes per line size exceeds the maximum value of B" );
  }

  if ( DEBUG_BUFFER_CONST ) {
    { // MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      this->debug_is_activated_();
      BUFFER << std::format( "buffer::allocate(allocation[{}], bytes_per_line[{}], ident[{}]) called ⇣", allocation,
                             bytes_per_line, ident )
             << '\n';
    }
  }

  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock( mtx_ );

    // Check if the buffer is already allocated
    if ( !nuts_buffer_.empty() ) {
      if ( DEBUG_BUFFER_CONST ) {
        BUFFER << "  but the buffer is already allocated. it will throw a logic::error" << '\n';
      }
      throw std::logic_error( "Buffer is already allocated." );
    }

    nuts_buffer_unlined_ = nuts_buffer_unlined_t( bytes_per_line, nuts_byte_ );
    nuts_buffer_ = nuts_buffer_t( allocation, std::move( nuts_buffer_unlined_ ) );

    // clear nuts_unlined_buffer_ and release unused memory.
    nuts_buffer_unlined_.clear();
    nuts_buffer_unlined_.shrink_to_fit();

    // Insert metadata into the registry
    BUFFER << "  nuts_buffer_ address " << addr_hex_() << " (nuts_buffer_) -> " << ident << '\n';
    insert_metadata_( addr_hex_(), std::nullopt, ident );
    set_allocated_();
    registry_->insert( std::make_pair(
      ident, nuts_buffer_stored_t{ std::move( nuts_buffer_ ), get_allocated_(), std::move( metadata_ ) } ) );
    unset_allocated_();

    // reset metadata_
    metadata_ = {};

    // clear nuts_buffer_ and release unused memory.
    nuts_buffer_.clear();
    nuts_buffer_.shrink_to_fit();

    BUFFER << "  registry_ metadata address -> " << std::get<0>( registry_->at( ident ).metadata.value() ) << '\n';
  }
}

}
