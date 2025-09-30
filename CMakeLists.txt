cmake_minimum_required(VERSION 3.10)
project(NESCPP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the policy to avoid the warning
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler-specific flags
if(MSVC)
    # MSVC flags
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG /MD")
else()
    # GCC/Clang flags
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Separate your main executable and tests
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE TESTS "tests/*.cpp")

message(STATUS "Tests files found:")
foreach(testfile ${TESTS})
    message(STATUS "  ${testfile}")
endforeach()

# Create main executable (without tests)
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")  # Remove main.cpp for library
add_library(NESCPPLib ${SOURCES})
target_include_directories(NESCPPLib PUBLIC include)

# Apply compiler-specific debug flags to library
if(MSVC)
    target_compile_options(NESCPPLib PRIVATE 
        $<$<CONFIG:Debug>:/Zi /Od>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(NESCPPLib PRIVATE 
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3>
    )
endif()

# Create main executable
add_executable(NESCPP src/main.cpp)
target_link_libraries(NESCPP NESCPPLib)

# Apply compiler-specific debug flags to main executable
if(MSVC)
    target_compile_options(NESCPP PRIVATE 
        $<$<CONFIG:Debug>:/Zi /Od>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(NESCPP PRIVATE 
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3>
    )
endif()

# Create test executable
add_executable(NESCPPTests ${TESTS})
target_link_libraries(NESCPPTests
    NESCPPLib
    gtest_main
    gtest
)
target_include_directories(NESCPPTests PRIVATE include)

# Apply compiler-specific debug flags to test executable
if(MSVC)
    target_compile_options(NESCPPTests PRIVATE 
        $<$<CONFIG:Debug>:/Zi /Od>
        $<$<CONFIG:Release>:/O2>
    )
else()
    target_compile_options(NESCPPTests PRIVATE 
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3>
    )
endif()

# Enable testing
enable_testing()
include(GoogleTest)
gtest_discover_tests(NESCPPTests)