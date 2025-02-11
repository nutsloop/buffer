#pragma once

// MARK: (buffer) buffer debug pre-processor
#ifndef DEBUG_BUFFER
// HINT: default value if not defined elsewhere
// HINT: set to false also to find out what include c++ std headers are needed.
#define DEBUG_BUFFER true
#endif

#if DEBUG_BUFFER == false

// Mock macros to provide `<<` compatibility
#include <sstream>

#define BUFFER std::ostream(nullptr)       // No-op stream
#define BUFFER_WARN std::ostream(nullptr)  // No-op stream
#define BUFFER_ERROR std::ostream(nullptr) // No-op stream

#else

#include "log.h++"
#warning "DEBUG_BUFFER is enabled"


#define BUFFER log::stream("buffer", __FILE__, __LINE__, nutsloop::Level::INFO)
#define BUFFER_WARN                                                            \
log::stream("buffer", __FILE__, __LINE__, nutsloop::Level::WARN)
#define BUFFER_ERROR                                                           \
log::stream("buffer", __FILE__, __LINE__, nutsloop::Level::ERROR)

namespace nutsloop::nbuffer{

class internal_debug {

public:
  internal_debug();
  ~internal_debug();
};
}

#endif
