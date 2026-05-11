cmake_minimum_required(VERSION 3.10)
project(prime_srp)

set(CMAKE_CXX_STANDARD 17)

add_executable(main src/main.cpp src/PrimeCharacteristics.cpp)

target_link_libraries(main primesieve gmp pari)

target_include_directories(main PRIVATE include)