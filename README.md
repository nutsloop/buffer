# nutsloop::buffer

`nutsloop::buffer` is a high-performance C++23 library for managing memory buffers. 
Designed with flexibility and extensibility in mind, it provides powerful tools for handling data buffers with optional 
metadata, registry management, and debugging capabilities.  
This library is particularly suited for systems that require efficient memory operations or buffer initialization.

___

## Features

- **Advanced Buffer Management:**
    - Supports buffers with lines and multi-dimensional structures.
    - Allows metadata to associate buffers with sources (e.g., files or libraries).
- **Robust Registry System:**
    - Includes optional buffer registry for advanced use cases.
    - Metadata helps track memory allocation and usage through registries.
- **Multi-threading Friendly:**
    - Implements thread-safe buffer operations with `std::shared_mutex`.
- **Type-safe Design:**
    - Concepts-based input validation with `ValidIntegerTypes` for functions requiring integers.
- **Configurable Debugging:**
    - Debugging tools activated via compile-time macros (e.g., `NUTSLOOP_BUFFER_DEBUG`).
    - Debug logs provide detailed information about internal operations (e.g., allocation, metadata management).
- **Flexible Initialization:**
    - Allocates buffers dynamically, optionally linked to registries or metadata.
    - Supports reading/writing buffers to/from files.

___

## Installation

### Requirements

- nutsloop::log library
- C++23 Compatible Compiler (e.g., GCC 13+, Clang 16+, or MSVC with C++23 support)
- CMake 3.25+ build system

### Steps

1. Clone the project:

   ```bash
   git clone https://github.com/nutsloop/buffer.git
   cd buffer
   ```

2. Clone and Build the nutsloop::log:

   ```bash
   mkdir deps && cd deps
   git clone https://github.com/nutsloop/log.git
   cd log
   cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
   cmake --build build/release
   ```

3. Build nutsloop::buffer as a library

   ```bash
   # back to the root directory
   cd ../../
   cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
   cmake --build build/release
   ```

   Link the library to your application. you can use the CMakeLists.txt 
   included in the repository, as an example.  

4. Build the main.c++ file included in the repository

   ```bash
   # back to the root directory
   cd ../../
   cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
   cmake --build build/debug
   ```

   run the example

   ```bash
   ./build/debug/buffer_
   ```

___

## Initialization and Usage

___

## Example Workflow

``` c++
#include <iostream>
#include "buffer.h++"

void example() {
  using namespace nutsloop;

  // Create a buffer with registry support
  buffer buf(true);

  // Allocate memory for the buffer
  buf.allocate_into("test_buffer", 100, 10);  // 100 lines of 10 bytes each

  std::cout << "Buffer and registry initialized successfully!" << std::endl;
}

int main() {
  example();
  return 0;
}
```

___

## Notes

1. **Thread Safety:** Thread-safe operations rely on `std::shared_mutex`; ensure proper synchronization when using buffers in concurrent contexts.
2. **Debugging:** Debugging can be customized using logging settings (`nutsloop::log::activate` and `log::set`).
3. **Registry Use Cases:** Registry provides tracking for buffer entities but is optional based on the constructor or use case.
4. **Metadata Accounting:** Metadata links buffers to memory addresses, file origins, or custom identifiers (and is crucial in registry-enabled workflows).

___

## Contributions

Contributions are welcome! Open an issue or submit a PR to propose enhancements or report bugs.

___

## License

This library is licensed under the **Apache-2.0**.
