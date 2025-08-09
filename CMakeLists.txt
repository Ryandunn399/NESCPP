cmake_minimum_required(VERSION 3.10)
project(NESCPP)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Separate your main executable and tests
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB TESTS "tests/*.cpp")

# Create main executable (without tests)
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")  # Remove main.cpp for library
add_library(NESCPPLib ${SOURCES})
target_include_directories(NESCPPLib PUBLIC include)

# Create main executable
add_executable(NESCPP src/main.cpp)
target_link_libraries(NESCPP NESCPPLib)

# Create test executable
add_executable(NESCPPTests ${TESTS})
target_link_libraries(NESCPPTests 
    NESCPPLib
    gtest_main
    gtest
)
target_include_directories(NESCPPTests PRIVATE include)

# Enable testing
enable_testing()
include(GoogleTest)
gtest_discover_tests(NESCPPTests)