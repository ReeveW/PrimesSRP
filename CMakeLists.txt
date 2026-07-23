cmake_minimum_required(VERSION 3.10)

project(prime_srp)

enable_testing()

set(CMAKE_CXX_STANDARD 17)

function(add_prime_variant name)
  add_library(${name}_lib
    src/PrimeCharacteristics.cpp
    src/PrimeGeneration.cpp
    src/Denominators.cpp
  )

  target_include_directories(${name}_lib PUBLIC include)

  target_link_directories(${name}_lib PRIVATE
    $ENV{HOME}/.local/lib64
  )

  set_property(TARGET ${name}_lib PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

  target_compile_options(${name}_lib PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Release>:-march=native>
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Debug>:-g>
  )

  target_compile_definitions(${name}_lib PRIVATE ${ARGN})

  target_link_libraries(${name}_lib
    primesieve
    gmp
    pthread
  )

  add_executable(${name} src/main.cpp)
  target_compile_definitions(${name} PRIVATE ${ARGN})
  target_link_libraries(${name} ${name}_lib)
endfunction()

add_prime_variant(main_all THETA_DATA FIRST_PRIME LARGEST_GAP)

add_prime_variant(main_theta_data THETA_DATA)
add_prime_variant(main_first_prime FIRST_PRIME)
add_prime_variant(main_largest_gap LARGEST_GAP)

add_executable(test_runner tests/test.cpp)
target_link_libraries(test_runner main_all_lib)
target_compile_definitions(test_runner PRIVATE THETA_DATA FIRST_PRIME LARGEST_GAP)

add_test(NAME main_test COMMAND test_runner)