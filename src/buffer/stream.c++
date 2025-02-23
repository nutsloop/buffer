#include "buffer.h++"

namespace nutsloop {

buffer::stream_ buffer::stream() {
#if DEBUG_BUFFER == true
  BUFFER << '\n'
         << "  buffer::stream() called ⇣"_.green().bold() << '\n'
         << "    initializing buffer::stream_ class"_.magenta() << '\n'
         << ansi("    with buffer @{}", std::format("0x{:x}", reinterpret_cast<uintptr_t>(this)))
                .background_magenta()
                .white()
                .bold()
         << '\n';

#endif

  return stream_(this);
}

} // namespace nutsloop
