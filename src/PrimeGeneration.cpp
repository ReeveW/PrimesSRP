#include "PrimeGeneration.hpp"

std::vector<uint64_t> primeList;

uint64_t PreComputedPrimeIterator::power(uint64_t base, uint64_t power) {
  uint64_t ans = 1;
  for (uint64_t i = 0; i < power; ++i) {
    ans *= base;
  }
  return ans;
}

void PreComputedPrimeIterator::fillPrimeList(uint64_t x) {
  uint64_t approximatePrimeCount =
      x / (std::log(static_cast<long double>(x)) - 1.0L);
  primeList.reserve(approximatePrimeCount);
  primesieve::generate_primes(0, x, &primeList);
  return;
}

struct Compare {
  bool operator()(const primePowerElement& a,
                  const primePowerElement& b) const {
    return a.value > b.value;
  }
};

void PreComputedPrimeIterator::fillPrimeListWithPowers(uint64_t x) {
  primesieve::iterator it;
  uint64_t prime;
  std::vector<std::queue<primePowerElement>> primePowerQueues(
      static_cast<uint>(std::log2(x * 1.0L)) + 1);
  uint64_t val;
  while ((prime = it.next_prime()) < x) {
    // do i have to worry about integer overflow here?
    for (uint64_t p = 1; (val = power(prime, p)) < x; ++p) {
      primePowerQueues[p].push({val, p});
    }
  }

  std::priority_queue<primePowerElement, std::vector<primePowerElement>,
                      Compare>
      heap;
  uint64_t count = 0;
  for (auto& x : primePowerQueues) {
    count += x.size();
    if (!x.empty()) {
      heap.push(x.front());
      x.pop();
    }
  }
  primeList.reserve(count);

  while (heap.size() > 0) {
    primeList.push_back(heap.top().value);
    uint32_t list = heap.top().power;
    heap.pop();
    if (!primePowerQueues[list].empty()) {
      heap.push(primePowerQueues[list].front());
      primePowerQueues[list].pop();
    }
  }
  return;
}

PreComputedPrimeIterator::PreComputedPrimeIterator(uint64_t x, bool primePowers) {
  if(!primeList.empty()){
    return;
  }
  if(primePowers){
    PreComputedPrimeIterator::fillPrimeListWithPowers(x);
  }else{
    PreComputedPrimeIterator::fillPrimeList(x);
  }
  return;
}

uint64_t PreComputedPrimeIterator::next_prime() {
  if(index >= primeList.size()){
    return UINT64_MAX;
  }
  return primeList[index++];
}
