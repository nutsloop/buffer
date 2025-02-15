cmake_minimum_required(VERSION 3.30)
project(buffer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CMAKE_BINARY_DIR should be build
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

option(USE_RELATIVE_PATHS "Use relative paths for libraries and includes" OFF)
option(BUILD_SHARED_LIBS "Build a shared library" OFF)
option(BUILD_MAIN_EXE "Build the main executable include in the repository for testing purpose" OFF)

# tests options
option(BUFFER_TEST "Build executable to test the buffer() constructors" OFF)
option(SIZE_TEST "Build executable to test the buffer.size() method" OFF)

# Configure based on the switch
if(USE_RELATIVE_PATHS)
    set(LOG_LIB_DIR "../log/build/release")
    set(LOG_ROOT_INCLUDE_DIR "../log/include")
    set(ANSI_ROOT_INCLUDE_DIR "../ansi/include")
else()
    set(LOG_LIB_DIR "${CMAKE_SOURCE_DIR}/deps/log/build/release")
    set(LOG_ROOT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/log/include")
    set(ANSI_ROOT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/ansi/include")
endif()

# Root include directory
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include ${LOG_ROOT_INCLUDE_DIR} ${ANSI_ROOT_INCLUDE_DIR})

# Source files
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.c++)

# Set output directories for executables and libraries
# Define output directories for executables and libraries
# The ${CMAKE_BINARY_DIR} changes dynamically based on the build directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Optimization flags
set(OPT_FLAGS "-O3 -march=native -flto")
set(DEBUG_FLAGS "-g -O0 -fno-omit-frame-pointer -DDEBUG_BUFFER=true -DDEBUG_LOG=false -DEXPERIMENTAL_INSTANCE=true")
set(RELEASE_FLAGS "${OPT_FLAGS} -DNDEBUG -DDEBUG_BUFFER=false -DDEBUG_LOG=false -DEXPERIMENTAL_INSTANCE=true")
set(RELWITHDEBINFO_FLAGS "${OPT_FLAGS} -g -DNDEBUG -DDEBUG_BUFFER=false -DDEBUG_LOG=false -DEXPERIMENTAL_INSTANCE=true")
set(EXTRA_FLAGS "-fexperimental-library")

# Combine flags for each build type
set(CMAKE_CXX_FLAGS_DEBUG "${DEBUG_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${RELEASE_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${RELWITHDEBINFO_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${RELEASE_FLAGS} -Os ${EXTRA_FLAGS}")

# Add the library
add_library(${PROJECT_NAME} buffer.h++ ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

# Link the external static library
# Link the external library and add its include path
target_link_libraries(${PROJECT_NAME} PUBLIC ${LOG_LIB_DIR}/liblog.a)

# Optional: Add SO versioning for shared libraries
if (BUILD_SHARED_LIBS)
    set_target_properties(${PROJECT_NAME} PROPERTIES
      VERSION 0.0.1
      SOVERSION 0
    )
endif ()

# Add the executable for testing purposes only if BUILD_MAIN_EXE is set to ON
if (BUILD_MAIN_EXE)
    add_executable(_buffer main.c++)
    target_link_libraries(_buffer PRIVATE ${PROJECT_NAME})
endif ()

if (SIZE_TEST)
    add_executable(size_test tests/buffer/size.c++)
    target_link_libraries(size_test PRIVATE ${PROJECT_NAME})
endif ()

if (BUFFER_TEST)
    add_executable(buffer_test tests/buffer/buffer.c++)
    target_link_libraries(buffer_test PRIVATE ${PROJECT_NAME})
endif ()
