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
#include <utility>
#include <vector>

namespace nutsloop::nbuffer::types {

// MARK: (buffer) buffer types
// HINT: buffer
//       a byte is a vector of byte
//       a line is a vector of vector of byte
//       a file is a vector of vector of vector of byte
using nuts_byte_t = std::vector<std::byte>;
using nuts_buffer_unlined_t = std::vector<nuts_byte_t>;
using nuts_buffer_t = std::vector<nuts_buffer_unlined_t>;

// MARK: (buffer) buffer metadata
// HINT: std::get<0> -> std::string => memory address
// HINT: std::get<1> -> std::optional<std::string> => created from file (filename)
// HINT: std::get<2> -> std::optional<std::string> => part of library (identifier)
struct nuts_buffer_metadata_t {
  uintptr_t memory_address{};
  std::optional<std::filesystem::path> path_to_file;
  std::optional<std::string> registry_identifier;
  nuts_buffer_metadata_t() = default;
  nuts_buffer_metadata_t(const uintptr_t memory_address,
                         const std::optional<std::string> &path_to_file,
                         std::optional<std::string> registry_identifier)
      : memory_address(memory_address),
        path_to_file(path_to_file),
        registry_identifier(std::move(registry_identifier)) {}
  [[nodiscard]] uintptr_t get_memory_address() const { return memory_address; }
  [[nodiscard]] std::optional<std::string> get_path_to_file() const { return path_to_file; }
  [[nodiscard]] std::optional<std::string> get_registry_identifier() const {
    return registry_identifier;
  }
};

// MARK: (buffer) buffer stream
using nuts_buffer_stream_t = std::optional<std::tuple<std::array<std::size_t, 2>, nuts_byte_t>>;
using nuts_buffer_stream_diff_t = std::vector<std::byte>::difference_type;
// MARK: (buffer) size types
struct nuts_buffer_col_t {
  std::size_t col_offset;
  std::size_t size;
};
struct nuts_buffer_line_t {
  std::size_t line_offset;
  std::size_t size;
  std::vector<nuts_buffer_col_t> cols;
};
struct nuts_buffer_size_t {
  std::size_t size;
  std::vector<nuts_buffer_line_t> lines;
};

// HINT: a container of buffers (not implemented yet)
struct nuts_buffer_stored_t {
  nuts_buffer_t buffer;
  bool is_allocated;
  std::optional<nuts_buffer_metadata_t> metadata;
};
using nuts_buffer_registry_t = std::unordered_map<std::string, nuts_buffer_stored_t>;

// HINT: view on buffer (not implemented yet)
using nuts_buffer_view_unlined_t = std::span<std::byte>;
using nuts_buffer_view_t = std::span<nuts_buffer_view_unlined_t>;

// HINT: read-only buffer (not implemented yet)
template <std::size_t S> using nuts_buffer_ro_unlined_t = std::array<std::byte, S>;
template <std::size_t S, std::size_t I>
using nuts_buffer_ro_t = std::array<nuts_buffer_ro_unlined_t<I>, S>;

} // namespace nutsloop::nbuffer::types
