#pragma once
#include "buffer/internal_debug.h++"

#include "types.h++"

#include <atomic>
#include <shared_mutex>

namespace nutsloop {

using namespace nbuffer::types;

class buffer {

  // MARK: (buffer) stream class
  class stream_ {
  public:
    explicit stream_(buffer *buf_ref) : buffer_(buf_ref) {}
    // MARK: (buffer) buffer stream methods
    nuts_buffer_stream_t next(bool strip_null_byte = false);
    std::optional<nuts_byte_t> next(std::size_t search_at_line, std::size_t from_col_n,
                                    const nuts_byte_t &until_it_finds);
    nuts_buffer_stream_t next(size_t line, bool strip_null_byte = false);

    // moving into the buffer.
    std::size_t move_at_line(std::size_t line_n);
    std::size_t move_at_column(std::size_t col_n);
    std::size_t ends();

    // editing the buffer.
    void operator-(std::optional<std::tuple<std::size_t>> coordinates);
    void operator+(const nuts_byte_t &byte);
    stream_ &operator=(const nuts_byte_t &byte);

  private:
    static nuts_buffer_stream_diff_t diff_(const std::atomic<std::size_t> &index);
    friend class buffer;
    std::shared_mutex mtx_;
    std::atomic<std::size_t> line_{0};
    std::atomic<std::size_t> column_{0};
    nuts_byte_t byte_ = nuts_byte_t{0x00};
    buffer *buffer_;
    // null
    nuts_byte_t null_byte_{std::byte{0x00}};
    // newline
    nuts_byte_t newline_byte_{std::byte{0x0A}};
  };
  friend class stream_;

public:
  buffer();
  // create a single line buffer from a string.
  explicit buffer(const std::string &str);
  // create a buffer form file.
  explicit buffer(const std::filesystem::path &file_path);

  // MARK: (buffer) buffer allocation.
  void allocate();

  // MARK: (buffer) buffer from file and string.
  void read(const std::filesystem::path &file_path);
  void string(const std::string &str);

  void write(const std::optional<std::filesystem::path> &file_path);

  // MARK: (buffer) buffer stream
  stream_ stream();

  nuts_buffer_t &get();
  nuts_buffer_unlined_t &get(const size_t &line);
  nuts_byte_t &get(const size_t &line, const size_t &col);

  // GOOD: size methods
  [[nodiscard]] std::size_t size() const;
  [[nodiscard]] std::size_t size(std::size_t line, bool strip_null_byte = false) const;
  [[nodiscard]] std::size_t size(std::size_t line, std::size_t col) const;
  [[nodiscard]] nuts_buffer_size_t sizes(bool strip_null_byte = false) const;

  // MARK: (buffer) metadata methods
  nuts_buffer_metadata_t metadata() const;

  std::string to_string() const;
  std::string to_string(std::size_t line) const;
  std::string to_string(std::size_t line, std::size_t col) const;

  char ch(std::byte b);
  nuts_byte_t byte(const std::string &str);
  nuts_byte_t byte(char c);
  std::string byte(const nuts_byte_t &byte);

  void reset(bool registry = true);
  // HINT: all the methods below are not implemented yet.
  explicit buffer(bool has_registry);
  buffer(std::size_t allocation, std::size_t bytes_per_line);
  void allocate(std::size_t allocation, std::size_t bytes_per_line);
  void allocate_into(std::string ident, std::size_t allocation, std::size_t bytes_per_line);
  void registry();
  void registry(std::string &ident);
  void read_into(std::string ident, const std::filesystem::path &file_path);
  void read_into(std::string ident, const std::filesystem::path &file_path, const int &fd,
                 const off_t &file_size);

  // ONGOING: experimentation. overload '<<' and '>>' operators, for insertion & deletion.
  // Overload << for insertion
  buffer &operator<<(const std::tuple<size_t, size_t, nuts_byte_t> &insertion);
  // Overload >> for deletion
  buffer &operator>>(const std::tuple<size_t, std::optional<size_t>> &deletion);

private:
  // MARK: (buffer) buffer process raw bytes
  void process_bytes_(const std::string &str);
  static std::size_t char_length_(const unsigned char &byte);
  static bool continuation_byte_(const std::string &str, std::size_t offset,
                                 std::size_t char_length);
  void from_string_(const std::string &str);
  void from_file_(const std::filesystem::path &file_path);

  // MARK: (buffer) mutex methods and fields
  mutable std::shared_mutex mtx_;

  nuts_buffer_t nuts_buffer_;
  nuts_buffer_unlined_t nuts_buffer_unlined_;
  nuts_byte_t nuts_byte_;
  std::byte nutsbyte_{0x00};

  // predefined nuts_byte_t
  // null
  nuts_byte_t null_byte_{std::byte{0x00}};
  // newline
  nuts_byte_t newline_byte_{std::byte{0x0A}};
  // carriage return
  nuts_byte_t carriage_return_byte_{std::byte{0x0D}};
  // tab
  nuts_byte_t tab_byte_{std::byte{0x09}};
  // space
  nuts_byte_t space_byte_{std::byte{0x20}};
  // malformed
  nuts_byte_t malformed_byte_{std::byte{0xEF}, std::byte{0xBF}, std::byte{0xBD}};

  uintptr_t addr_hex_();

  bool get_allocated_();
  void set_allocated_();
  void unset_allocated_();
  std::atomic<bool> allocated_{false};

  // MARK: (buffer) stream controls

  // MARK: (buffer) metadata methods and fields
  nuts_buffer_metadata_t metadata_{};

  // type of buffer
  bool get_manual_() const;
  void set_manual_();
  void unset_manual_();
  std::atomic<bool> manual_{false};

  bool get_from_string_() const;
  void set_from_string_();
  void unset_from_string_();
  std::atomic<bool> string_{false};

  bool get_read_() const;
  void set_read_();
  void unset_read_();
  std::atomic<bool> read_{false};

  // MARK: (buffer) registry methods and fields
  bool get_has_registry_();
  void set_has_registry_();
  void unset_has_registry_();
  std::atomic<bool> has_registry_{false};
  std::unique_ptr<nuts_buffer_registry_t> registry_{nullptr};

#if DEBUG_BUFFER == true
  // MARK: (buffer) buffer debug methods and fields
  void set_internal_debug_();
  std::unique_ptr<nbuffer::internal_debug> internal_debug_{nullptr};
#endif
};

} // namespace nutsloop
