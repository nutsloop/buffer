#include "buffer.h++"

namespace nutsloop {

// ONGOING: experimentation.
// Overload << for insertion
buffer &buffer::operator<<(const std::tuple<size_t, size_t, nuts_byte_t> &insertion) {
  const size_t line = std::get<0>(insertion);                           // Line index
  const auto col = static_cast<std::ptrdiff_t>(std::get<1>(insertion)); // Column index
  const nuts_byte_t value = std::get<2>(insertion);                     // Byte value to insert

  if (line >= nuts_buffer_.size())
    throw std::out_of_range("Line index out of range");

  auto &target_line = nuts_buffer_[line];
  if (col > target_line.size()) // Allow appending at the end
    throw std::out_of_range("Column index out of range");

  target_line.insert(target_line.begin() + col, value);
  return *this;
}

// Overload >> for deletion
buffer &buffer::operator>>(const std::tuple<size_t, std::optional<size_t>> &deletion) {
  const auto line = static_cast<std::ptrdiff_t>(std::get<0>(deletion)); // Line index

  BUFFER << "INTO OPERATOR >>" << '\n';
  if (line >= nuts_buffer_.size())
    throw std::out_of_range("Line index out of range");

  if (std::get<1>(deletion).has_value()) {
    // Column index provided: delete a specific character
    const auto col = static_cast<std::ptrdiff_t>(std::get<1>(deletion).value());
    auto &target_line = nuts_buffer_[line];

    if (col >= target_line.size())
      throw std::out_of_range("Column index out of range");

    target_line.erase(target_line.begin() + col);
  } else {
    // No column index: delete the entire line
    nuts_buffer_.erase(nuts_buffer_.begin() + line);
  }

  return *this;
}

void buffer::operator-(const nuts_operators_line_t &line) {

  if (line.y >= nuts_buffer_.size()) {
    throw std::out_of_range("Line index out of range");
  }

  nuts_buffer_.erase(nuts_buffer_.begin() + diff_(line.y));
}

void buffer::operator-(const nuts_operators_col_t &col) {

  if (col.y >= nuts_buffer_.size()) {
    throw std::out_of_range("Line index out of range");
  }

  auto &target_line = nuts_buffer_[col.y];
  if (col.x >= target_line.size()) {
    throw std::out_of_range("Column index out of range");
  }
  target_line.erase(target_line.begin() + diff_(col.x));
}

void buffer::operator+(const nuts_operators_line_t &line) {

  if (line.y >= nuts_buffer_.size()) {
    throw std::out_of_range("Line index out of range");
  }

  nuts_buffer_.insert(nuts_buffer_.begin() + diff_(line.y), nuts_buffer_unlined_t{null_byte_});
}

void buffer::operator+(const nuts_operators_col_t &col) {

  if (col.y >= nuts_buffer_.size()) {
    throw std::out_of_range("Line index out of range");
  }

  auto &target_line = nuts_buffer_[col.y];

  target_line.insert(target_line.begin() + diff_(col.x), null_byte_);
}

void buffer::operator+(const nuts_operators_byte_t &byte) {

  if (byte.y >= nuts_buffer_.size()) {
    throw std::out_of_range("Line index out of range");
  }

  auto &target_line = nuts_buffer_[byte.y];
  if (byte.x >= target_line.size()) {
    throw std::out_of_range("Column index out of range");
  }
  target_line.insert(target_line.begin() + diff_(byte.x), byte.byte);
}

buffer &buffer::operator=(const nuts_operators_byte_t &byte) {

  if (byte.y >= nuts_buffer_.size()) {
    throw std::out_of_range("Line index out of range");
  }

  auto &target_line = nuts_buffer_[byte.y];
  if (byte.x >= target_line.size()) {
    throw std::out_of_range("Column index out of range");
  }

  target_line[byte.x] = byte.byte;

  return *this;
}

nuts_diff_t buffer::diff_(const std::size_t &index) { return static_cast<nuts_diff_t>(index); }

} // namespace nutsloop
