#pragma once

#include <primesieve.hpp>
#include <queue>
#include <vector>

struct primePowerElement {
  uint64_t value, power;
};

extern std::vector<uint64_t> primeList;

class PreComputedPrimeIterator {
 private:
  uint64_t index = 0;

  void fillPrimeListWithPowers(uint64_t x);
  void fillPrimeList(uint64_t x);
  uint64_t power(uint64_t base, uint64_t power);

 public:
  PreComputedPrimeIterator() = default;
  PreComputedPrimeIterator(uint64_t x, bool primePowers = false);
  uint64_t next_prime();
};
