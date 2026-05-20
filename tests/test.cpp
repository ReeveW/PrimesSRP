#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

#include "PrimeCharacteristics.hpp"

// tests that the number of primes in all APs matches known value for 10^10.
// value 455052511 is from here: https://t5k.org/howmany.html
// this test takes about 7.5 seconds to run with x as 10^10.
int main() {
  std::vector<uint64_t> count = primesModN(1000, 10000000000);
  assert(add(count) == 455052511);

  return 0;
}