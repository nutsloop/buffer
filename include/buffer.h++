#pragma once
#include "log.h++"

#include <array>
#include <atomic>
#include <filesystem>
#include <format>
#include <optional>
#include <shared_mutex>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#define BUFFER log::stream( "buffer", __FILE__, __LINE__, 'I' )
#define BUFFER_WARN log::stream( "buffer", __FILE__, __LINE__, 'W' )
#define BUFFER_ERROR log::stream( "buffer", __FILE__, __LINE__, 'E' )

namespace nutsloop{

constexpr bool DEBUG = true;

// MARK: (buffer) integer types
// HINT: shorts for integer types
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// MARK: (buffer) buffer types
// HINT: buffer
//       a byte is a single byte
//       a line is a vector of byte
//       a file is a vector of vector of byte
using nuts_byte_t = std::byte;
using nuts_buffer_unlined_t = std::vector<nuts_byte_t>;
using nuts_buffer_t = std::vector<nuts_buffer_unlined_t>;

// MARK: (buffer) buffer metadata
// HINT: std::get<0> -> std::string => memory address
// HINT: std::get<1> -> std::optional<std::string> => created from file (filename)
// HINT: std::get<2> -> std::optional<std::string> => part of library (identifier)
using nuts_buffer_mem_addr_t = std::string;
using nuts_buffer_from_file_t = std::optional<std::string>;
using nuts_buffer_registry_identifier_t = std::optional<std::string>;
using nuts_buffer_metadata_t = std::tuple<nuts_buffer_mem_addr_t, nuts_buffer_from_file_t, nuts_buffer_registry_identifier_t>;

// HINT: a container of buffers (not implemented yet)
struct nuts_buffer_stored_t {
  nuts_buffer_t buffer;
  std::optional<nuts_buffer_metadata_t> metadata;
};
using nuts_buffer_registry_t = std::unordered_map<std::string, nuts_buffer_stored_t>;

// HINT: view on buffer (not implemented yet)
using nuts_buffer_view_unlined_t = std::span<nuts_byte_t>;
using nuts_buffer_view_t = std::span<nuts_buffer_view_unlined_t>;

// HINT: read-only buffer (not implemented yet)
template<std::size_t S>
using nuts_buffer_ro_unlined_t = std::array<nuts_byte_t, S>;
template<std::size_t S, std::size_t I>
using nuts_buffer_ro_t = std::array<nuts_buffer_ro_unlined_t<I>, S>;

// MARK: (buffer) buffer concepts

/**
 * @brief Concept for validating integer types.
 *
 * The `ValidIntegerTypes_C` concept ensures that the type `T`
 * is one of the following allowed integer types:
 * - `u8`  (8-bit unsigned integer)
 * - `u16` (16-bit unsigned integer)
 * - `u32` (32-bit unsigned integer)
 * - `u64` (64-bit unsigned integer)
 * - `size_t` (unsigned integer type representing size)
 *
 * This concept is used to restrict templates to work only with
 * the specified integer types for safer and more predictable behavior.
 *
 * @tparam T The type to be validated.
 */
template <typename T>
concept ValidIntegerTypes = std::same_as<T, u8> ||
  std::same_as<T, u16> ||
  std::same_as<T, u32> ||
  std::same_as<T, u64> ||
  std::same_as<T, size_t>;

class buffer{

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
   * @throws std::invalid_argument If the buffer has an existing registry, if allocation exceeds the maximum value of type
   * A, or if bytes per line exceed the maximum value of type B.
   */
  template <typename A = u8, typename B = u8>
  explicit buffer( A allocation = 1, B bytes_per_line = 1 )
    requires ValidIntegerTypes<A> && ValidIntegerTypes<B> {

    // Ensure both types are unsigned
    static_assert( std::is_unsigned_v<A>, "A must be an unsigned type" );
    static_assert( std::is_unsigned_v<B>, "B must be an unsigned type" );

    // Check allocation size validity
    constexpr A a_max_value = std::numeric_limits<A>::max();
    if ( allocation > a_max_value ) {
      throw std::invalid_argument(
        "Allocation size exceeds the maximum value of A" );
    }

    // Check bytes per line validity
    constexpr B b_max_value = std::numeric_limits<B>::max();
    if ( bytes_per_line > b_max_value ) {
      throw std::invalid_argument(
        "Bytes per line size exceeds the maximum value of B" );
    }

    if ( DEBUG ) {
      { // MARK (buffer) MUTEX LOCK
        std::shared_lock lock( mtx_ );
        this->debug_is_activated_();
        BUFFER << std::format(
            "buffer::buffer(allocation[{}], bytes_per_line[{}]) called ⇣"
            , allocation, bytes_per_line) << '\n';
      }
    }

    { // MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );
      nuts_buffer_unlined_ = nuts_buffer_unlined_t( bytes_per_line, nuts_byte_ );
      nuts_buffer_ = nuts_buffer_t( allocation, nuts_buffer_unlined_ );
      insert_metadata_( addr_hex_(), std::nullopt, std::nullopt );
    }
  }

  /**
   * Allocates memory for the buffer based on the specified allocation size and bytes per line.
   * Performs validation to ensure the parameters meet constraints and preconditions.
   * Throws exceptions if the buffer is already allocated or if the parameters are invalid.
   *
   * @param allocation The number of allocation units to reserve for the buffer. Must be of an unsigned type.
   * @param bytes_per_line The number of bytes attributed to each line of the buffer. Must be of an unsigned type.
   *
   * @throws std::invalid_argument If the buffer has a registry, or if the allocation size or bytes per line exceed their
   * respective maximum values.
   * @throws std::logic_error If the buffer is already allocated.
   */
  template <typename A = u8, typename B = u8>
  void allocate( A allocation = 1, B bytes_per_line = 1 )
    requires ValidIntegerTypes<A> && ValidIntegerTypes<B> {

    if ( get_has_registry_() ) {
      throw std::invalid_argument(
        "this buffer has a registry. use buffer::allocate_into() instead" );
    }

    // Ensure both types are unsigned
    static_assert( std::is_unsigned_v<A>, "A must be an unsigned type" );
    static_assert( std::is_unsigned_v<B>, "B must be an unsigned type" );

    // Check allocation size validity
    constexpr A a_max_value = std::numeric_limits<A>::max();
    if ( allocation > a_max_value ) {
      throw std::invalid_argument(
        "Allocation size exceeds the maximum value of A" );
    }

    // Check bytes per line validity
    constexpr B b_max_value = std::numeric_limits<B>::max();
    if ( bytes_per_line > b_max_value ) {
      throw std::invalid_argument(
        "Bytes per line size exceeds the maximum value of B" );
    }

    if ( DEBUG ) {
      { // MARK (buffer) MUTEX LOCK
        std::shared_lock lock( mtx_ );
        this->debug_is_activated_();
        BUFFER << std::format(
            "buffer::allocate(allocation[{}], bytes_per_line[{}]) called ⇣"
            , allocation, bytes_per_line ) << '\n';
      }
    }

    { // MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );

      // Check if the buffer is already allocated
      if ( !nuts_buffer_.empty() ) {
        if ( DEBUG ) {
            BUFFER << "  but the buffer is already allocated. it will throw a logic::error" << '\n';
        }
        throw std::logic_error( "Buffer is already allocated." );
      }

      nuts_buffer_unlined_ = nuts_buffer_unlined_t( bytes_per_line, nuts_byte_ );
      nuts_buffer_ = nuts_buffer_t( allocation, nuts_buffer_unlined_ );
    }
  }

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
    requires ValidIntegerTypes<A> && ValidIntegerTypes<B> {

    if ( !get_has_registry_() ) {
      throw std::invalid_argument(
        "registry_ is false. use buffer::buffer( true ) to initialize the registry" );
    }

    if ( registry_ == nullptr ) {
      throw std::invalid_argument(
        "registry_ is nullptr. use buffer::buffer( true ) to initialize the registry" );
    }

    // Ensure both types are unsigned
    static_assert( std::is_unsigned_v<A>, "A must be an unsigned type" );
    static_assert( std::is_unsigned_v<B>, "B must be an unsigned type" );

    // Check allocation size validity
    constexpr A a_max_value = std::numeric_limits<A>::max();
    if ( allocation > a_max_value ) {
      throw std::invalid_argument(
        "Allocation size exceeds the maximum value of A" );
    }

    // Check bytes per line validity
    constexpr B b_max_value = std::numeric_limits<B>::max();
    if ( bytes_per_line > b_max_value ) {
      throw std::invalid_argument(
        "Bytes per line size exceeds the maximum value of B" );
    }

    if ( DEBUG ) {
      { // MARK (buffer) MUTEX LOCK
        std::shared_lock lock( mtx_ );
        this->debug_is_activated_();
        BUFFER << std::format(
            "buffer::allocate(allocation[{}], bytes_per_line[{}], ident[{}]) called ⇣"
            , allocation, bytes_per_line, ident ) << '\n';
      }
    }

    { // MARK (buffer) MUTEX LOCK
      std::shared_lock lock( mtx_ );

      // Check if the buffer is already allocated
      if ( !nuts_buffer_.empty() ) {
        if ( DEBUG ) {
          BUFFER << "  but the buffer is already allocated. it will throw a logic::error" << '\n';
        }
        throw std::logic_error( "Buffer is already allocated." );
      }

      nuts_buffer_unlined_ = nuts_buffer_unlined_t( bytes_per_line, nuts_byte_ );
      nuts_buffer_ = nuts_buffer_t( allocation, std::move( nuts_buffer_unlined_) );

      // clear nuts_unlined_buffer_ and release unused memory.
      nuts_buffer_unlined_.clear();
      nuts_buffer_unlined_.shrink_to_fit();

      // Insert metadata into the registry
      BUFFER << "  nuts_buffer_ address " << addr_hex_() << " (nuts_buffer_) -> " << ident << '\n';
      insert_metadata_( addr_hex_(), std::nullopt, ident );

      registry_->insert( std::make_pair( ident,
        nuts_buffer_stored_t{ std::move(nuts_buffer_), std::move(metadata_) } )
      );

      // reset metadata_
      metadata_ = {};

      // clear nuts_buffer_ and release unused memory.
      nuts_buffer_.clear();
      nuts_buffer_.shrink_to_fit();

      BUFFER << "  registry_ metadata address -> " << std::get<0>(registry_->at( ident ).metadata.value()) << '\n';
    }
  }

  // MARK: (buffer) buffer registry methods
  // HINT: not implemented yet
  void registry();
  // HINT: not implemented yet
  void registry( std::string& ident );

  // MARK: (buffer) buffer from file methods
  void read( const std::filesystem::path& file_path );
  void read( const int& fd, const off_t& file_size );

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
  void insert_metadata_( const nuts_buffer_mem_addr_t& mem_addr, const nuts_buffer_from_file_t& filename, const nuts_buffer_registry_identifier_t& ident );
  nuts_buffer_metadata_t metadata_{};

  // MARK: (buffer) read file methods and fields
  bool get_read_();
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
  std::unique_ptr<nuts_buffer_registry_t> registry_ { nullptr };

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
};
}
