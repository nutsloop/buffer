#include "buffer.h++"

namespace nutsloop {

buffer::stream buffer::make_stream() {
#if DEBUG_BUFFER == true
  BUFFER << '\n'
         << "  buffer::make_stream() called ⇣"_.green().bold() << '\n'
         << "    initializing buffer::stream class"_.magenta() << '\n'
         << ansi("    with buffer @{}", std::format("0x{:x}", reinterpret_cast<uintptr_t>(this)))
                .background_magenta()
                .white()
                .bold()
         << '\n';

#endif

  return stream(this);
}

} // namespace nutsloop
