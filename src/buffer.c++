#include "buffer.h++"

// TODO: Check If All bytes in the buffer are ASCII (uint8_t-Only).
namespace nutsloop {

buffer::buffer() {
#if DEBUG_BUFFER == true
  set_internal_debug_();
  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock(mtx_);
    BUFFER
        << '\n'
        << "  buffer::buffer() called ⇣"_.green().bold() << '\n'
        << "    initializing an empty buffer instance without allocating any resources."_.magenta()
        << '\n';
  }
#endif
}

buffer::buffer(const bool has_registry) {
#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

  if (has_registry) {
    set_has_registry_();
    if (registry_ == nullptr) {
      registry_ = std::make_unique<nuts_buffer_registry_t>();
    }
  }

#if DEBUG_BUFFER == true
  { // MARK (buffer) MUTEX LOCK
    std::shared_lock lock(mtx_);
    const ansi has_registry_string = has_registry ? "true"_.blue().bold() : "false"_.red().bold();
    BUFFER
        << '\n'
        << ansi("  buffer::buffer([{}]) called ⇣", has_registry).green().bold() << '\n'
        << "    initializing an empty buffer instance without allocating any resources."_.magenta()
        << '\n'
        << "    has_registry -> [ " << has_registry_string << " ]"_.white().bold() << '\n';
  }
#endif
}

// ONGOING

buffer::buffer(const std::string &str) {
#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

  reset();

  for (std::size_t i = 0; i < str.size(); ++i) {
    if (i == str.size() - 1 && str[i] == '\0') break; // Ignore only trailing null
    nuts_buffer_unlined_.push_back(byte(str[i]));
  }

  // reset the byte to null.
  byte( 0x00 );
  nuts_buffer_unlined_.push_back(nuts_byte_);
  nuts_buffer_.emplace_back(nuts_buffer_unlined_);
  insert_metadata_(addr_hex_(), std::nullopt, std::nullopt);
  set_allocated_();
  set_from_string_();
}

buffer::buffer(const char *c_str) {
#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

  reset();

  const std::size_t len = std::strlen(c_str);
  for (std::size_t i = 0; i <= len; ++i) {  // <= ensures '\0' is included
    nuts_buffer_unlined_.push_back(byte(c_str[i]));
  }

  // reset the byte to null.
  byte( 0x00 );

  nuts_buffer_.emplace_back(nuts_buffer_unlined_);
  insert_metadata_(addr_hex_(), std::nullopt, std::nullopt);
  set_allocated_();
  set_from_string_();
}

buffer::buffer(char *c_str) {
#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

  reset();

  const std::size_t len = std::strlen(c_str);
  for (std::size_t i = 0; i <= len; ++i) {  // <= ensures '\0' is included
    nuts_buffer_unlined_.push_back(byte(c_str[i]));
  }

  // reset the byte to null.
  byte(0x00);

  nuts_buffer_.emplace_back(nuts_buffer_unlined_);
  insert_metadata_(addr_hex_(), std::nullopt, std::nullopt);
  set_allocated_();
  set_from_string_();

  delete[] c_str;
}

 buffer::buffer(nuts_buffer_unlined_t &bytes) {
#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

  reset();

  if (bytes.back() != byte(0x00)) {
    bytes.push_back(byte(0x00));
  }

  nuts_buffer_unlined_ = bytes;
  nuts_buffer_.emplace_back(nuts_buffer_unlined_);
  insert_metadata_(addr_hex_(), std::nullopt, std::nullopt);
  set_allocated_();

}

 buffer::buffer(nuts_buffer_t &data) {
#if DEBUG_BUFFER == true
  set_internal_debug_();
#endif

  reset();

  for (auto &line : data) {
    if (line.back() != byte(0x00)) {
      line.push_back( byte(0x00) );
    }
    nuts_buffer_unlined_.insert(nuts_buffer_unlined_.end(), line.begin(), line.end());
  }

  nuts_buffer_.emplace_back( nuts_buffer_unlined_ );
  insert_metadata_(addr_hex_(), std::nullopt, std::nullopt);
  set_allocated_();

}

} // namespace nutsloop
