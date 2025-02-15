#include "buffer.h++"

#include "ansi.h++"

namespace nutsloop {

nuts_buffer_size_t buffer::size() const {

  std::size_t size = nuts_buffer_.size();
#if DEBUG_BUFFER == true
  BUFFER << '\n'
         << "  buffer::size() called ⇣"_.green().bold() << '\n'
         << ansi("    nuts_buffer_ size -> [{}]", size).yellow() << '\n'
         << "    size is the amount of elements(lines) in the vector container"_.magenta() << '\n';
#endif

  return size;
}

nuts_buffer_size_line_t buffer::size(const std::size_t line, const bool strip_null_byte) const {

  if (line >= nuts_buffer_.size()) {
    std::size_t size = nuts_buffer_.size();

#if DEBUG_BUFFER == true
    BUFFER_ERROR << '\n'
                 << ansi("  buffer::size(line[{}], strip_null_byte[{}]) called ⇣", line,
                         strip_null_byte)
                        .red()
                        .bold()
                 << '\n'
                 << ansi("    nuts_buffer_ size -> [{}]", size).green() << '\n'
                 << ansi("    line -> [{}]", line).red() << '\n'
                 << "    throw std::out_of_range"_.bold() << '\n';
#endif

    throw std::out_of_range(std::format("line {} is out of range", line));
  }

  std::size_t size = nuts_buffer_[line].size() - (strip_null_byte ? 1 : 0);
  const ansi strip_null_byte_string =
      strip_null_byte ? "true"_.blue().bold() : "false"_.red().bold();

#if DEBUG_BUFFER == true
  BUFFER << '\n'
         << ansi("  buffer::size(line[{}], strip_null_byte[{}]) called ⇣", line, strip_null_byte)
                .green()
                .bold()
         << '\n'
         << "    strip_null_byte -> [" << strip_null_byte_string << "]" << '\n'
         << ansi("    nuts_buffer_unlined_ size -> [{}]", size).yellow() << '\n'
         << "    size " << (strip_null_byte ? "NOT " : "") << "includes the null byte"_.magenta()
         << '\n';
#endif

  return size;
}

nuts_buffer_sizes_t buffer::sizes(const bool strip_null_byte) const {

  const std::size_t size = nuts_buffer_.size();
  const ansi strip_null_byte_string =
      strip_null_byte ? "true"_.blue().bold() : "false"_.red().bold();
  std::vector<std::array<std::size_t, 2>> sizes(size);

#if DEBUG_BUFFER == true
  BUFFER << '\n'
         << ansi("  buffer::sizes(strip_null_byte[{}]) called ⇣", strip_null_byte).green().bold()
         << '\n'
         << "    strip_null_byte -> [" << strip_null_byte_string << "]" << '\n'
         << ansi("    nuts_buffer_ size -> [{}]", size).yellow() << '\n'
         << "    size is the amount of elements(lines) in the vector container"_.magenta() << '\n';
#endif

  for (std::size_t i = 0; i < size; ++i) {
    const std::size_t line_size = nuts_buffer_[i].size() - (strip_null_byte ? 1 : 0);
    sizes[i] = {i, line_size};
#if DEBUG_BUFFER == true
    BUFFER << '\n'
           << ansi("  buffer::sizes(strip_null_byte[{}]) ->> iterating and populating sizes ⇣",
                   strip_null_byte)
                  .green()
                  .bold()
           << '\n'
           << ansi("    nuts_buffer_unlined size @[{}]->[{}]", i, line_size).yellow() << '\n'
           << "    size " << (strip_null_byte ? "NOT " : "") << "includes the null byte"_.magenta()
           << '\n';
#endif
  }

  return sizes;
}

} // namespace nutsloop
