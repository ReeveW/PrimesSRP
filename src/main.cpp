#include <gmp.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <primesieve.hpp>
#include <vector>

#include "PrimeCharacteristics.hpp"

int main() {
  const auto start1{std::chrono::steady_clock::now()};

  std::vector<uint64_t> count = primesModN(1000, 6000);

  const auto finish1{std::chrono::steady_clock::now()};
  const std::chrono::duration<double> elapsed_seconds1{finish1 - start1};
  std::cout << "time: " << elapsed_seconds1.count() << std::endl;
  std::cout << "primes: " << add(count) << std::endl;

  return 0;
}
