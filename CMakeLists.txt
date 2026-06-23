cmake_minimum_required(VERSION 3.10)

project(prime_srp)

enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_library(prime_lib
    src/PrimeCharacteristics.cpp
    src/PrimeGeneration.cpp
)

target_include_directories(prime_lib PUBLIC include)

target_link_libraries(prime_lib
    primesieve
    gmp
)

# Main executable
add_executable(main src/main.cpp)

target_link_libraries(main prime_lib)

# Test executable
add_executable(test_runner tests/test.cpp)

target_link_libraries(test_runner prime_lib)

# Register tests
add_test(NAME main_test COMMAND test_runner)