cmake_minimum_required(VERSION 3.10)
project(ThreadSafeQueues)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    # Use clang on macOS as my system is arm64 mac
    message(STATUS "Compiling for macOS with clang")
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_MACOS")
    if(CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
        message(STATUS "Compiling for Apple Silicon (arm64)")
    endif()
elseif(UNIX)
    message(STATUS "Compiling for Linux with gcc")
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
endif()

# Usual cmpiler optimiations
add_compile_options(-Wall -Wextra -pedantic -Werror)



include_directories(${CMAKE_SOURCE_DIR}/lib/queues)
include_directories(${CMAKE_SOURCE_DIR}/test)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/test/options.cc
    ${CMAKE_SOURCE_DIR}/test/test_main.cc
)

# Add the executable
add_executable(test_threadsafequeues ${SOURCES})

# Add clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/cmake_install.cmake
    COMMENT "Removed all build files"
)