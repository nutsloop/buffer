#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace nutsloop::nbuffer::types {

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
using nuts_buffer_from_file_t = std::optional<std::filesystem::path>;
using nuts_buffer_registry_identifier_t = std::optional<std::string>;
using nuts_buffer_metadata_t = std::tuple<nuts_buffer_mem_addr_t, nuts_buffer_from_file_t, nuts_buffer_registry_identifier_t>;

// MARK: (buffer) buffer stream
struct nuts_stream_t {
  bool active;
  std::size_t offset;
  std::size_t size;
  nuts_byte_t byte;
};
using nuts_buffer_stream_t = std::optional<std::tuple<std::array<std::size_t, 2>, nuts_byte_t>>;
// HINT: a container of streams (not implemented yet)
using nuts_stream_registry_t = std::unordered_map<std::string, nuts_stream_t>;

// HINT: a container of buffers (not implemented yet)
struct nuts_buffer_stored_t {
  nuts_buffer_t buffer;
  bool is_allocated;
  std::optional<nuts_buffer_metadata_t> metadata;
};
using nuts_buffer_registry_t = std::unordered_map<std::string, nuts_buffer_stored_t>;

// HINT: view on buffer (not implemented yet)
using nuts_buffer_view_unlined_t = std::span<nuts_byte_t>;
using nuts_buffer_view_t = std::span<nuts_buffer_view_unlined_t>;

// HINT: read-only buffer (not implemented yet)
template <std::size_t S>
using nuts_buffer_ro_unlined_t = std::array<nuts_byte_t, S>;
template <std::size_t S, std::size_t I>
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

}

// MARK: (buffer) size types
using nuts_buffer_size_t = std::size_t;
using nuts_buffer_size_line_t = std::size_t;
using nuts_buffer_sizes_t = std::vector<std::array<std::size_t, 2>>;
