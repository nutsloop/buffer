#include "buffer.h++"

#include "color.h++"
using namespace nutsloop::ncolor;

namespace nutsloop {

std::size_t buffer::size() const {

  std::size_t size = nuts_buffer_.size();
#if DEBUG_BUFFER == true
  BUFFER << '\n'
         << "  buffer::size() called ⇣"_s.red().bold() << '\n'
         << std::format("    nuts_buffer_ size -> [ {} ]", size) << '\n'
         << "    size is the amount of elements(lines) in the vector container"_s.magenta() << '\n';
#endif

  return size;
}

std::size_t buffer::size(const std::size_t line) const {

  if (line >= nuts_buffer_.size()) {
    std::size_t size = nuts_buffer_.size();

#if DEBUG_BUFFER == true
    BUFFER_ERROR << '\n'
                 << color(std::format("  buffer::size(line[{}]) called ⇣", line)).red().bold()
                 << '\n'
                 << std::format("    nuts_buffer_ size -> [ {} ]", size) << '\n'
                 << std::format("    line -> [ {} ]", line) << '\n'
                 << std::format("    line is out of range [ {} ]", line) << '\n'
                 << "    throw std::out_of_range" << '\n';
#endif

    throw std::out_of_range(std::format("line {} is out of range", line));
  }

  std::size_t size = nuts_buffer_[line].size();

#if DEBUG_BUFFER == true
  BUFFER << '\n'
         << color(std::format("  buffer::size(line[{}]) called ⇣", line)).red().bold() << '\n'
         << std::format("    nuts_buffer_unlined_ size -> [ {} ]", size) << '\n'
         << "    size includes the null byte"_s.magenta() << '\n';
#endif

  return size;
}

std::size_t buffer::size_line_strip_null_byte(std::size_t line) const {
  if (line >= nuts_buffer_.size()) {
    std::size_t size = nuts_buffer_.size();

#if DEBUG_BUFFER == true
    BUFFER_ERROR << '\n'
                 << color(
                        std::format("  buffer::size_line_strip_null_byte(line[{}]) called ⇣", line))
                        .red()
                        .bold()
                 << '\n'
                 << std::format("    nuts_buffer_ size -> [ {} ]", size) << '\n'
                 << std::format("    line -> [ {} ]", line) << '\n'
                 << std::format("    line is out of range [ {} ]", line) << '\n'
                 << "    throw std::out_of_range" << '\n';
#endif

    throw std::out_of_range(std::format("line {} is out of range", line));
  }

  std::size_t size = nuts_buffer_[line].size() - 1;

#if DEBUG_BUFFER == true
  BUFFER << '\n'
         << color(std::format("  buffer::size_line_strip_null_byte(line[{}]) called ⇣", line))
                .red()
                .bold()
         << '\n'
         << std::format("    nuts_buffer_unlined_ size -> [ {} ]", size) << '\n'
         << "    size NOT includes the null byte"_s.magenta() << '\n';
#endif

  return size;
}

} // namespace nutsloop
