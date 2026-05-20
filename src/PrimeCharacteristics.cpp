#include "PrimeCharacteristics.hpp"

uint64_t largestGap(const std::vector<uint64_t>& primes) {
  if (primes.size() <= 1) {
    return 0;
  }
  uint64_t gap = 0;
  for (int i = 0; i < primes.size() - 1; ++i) {
    gap = std::max(gap, primes[i + 1] - primes[i]);
  }
  return gap;
}

std::vector<uint64_t> primesModN(const uint64_t n, const uint64_t x) {
  primesieve::iterator it;
  uint64_t prime;

  std::vector<uint64_t> firstPrimeInAP(n);
  std::vector<uint64_t> largestGapInAP(n, 0); // tracks largest number of "steps"
  std::vector<uint64_t> primeCountInAP(n, 0);
  std::vector<uint64_t> lastPrimeInAP(n, 0);

  while ((prime = it.next_prime()) < x) {
    uint64_t a = prime % n;
    if (primeCountInAP[a] == 0) {
      firstPrimeInAP[a] = prime;
    } else {
      largestGapInAP[a] = std::max(largestGapInAP[a], ((prime / n) - lastPrimeInAP[a]));
    }
    primeCountInAP[a]++;
    lastPrimeInAP[a] = prime / n;
  }
  return primeCountInAP;
}

void computeAll(const uint64_t upperBound, const uint64_t x){
  for(int i = 2; i < upperBound; ++i){
    primesModN(i, x); // how should i be storing this information now?
  }
}

int add(const std::vector<uint64_t>& v) {
  int a = 0;
  for (int i = 0; i < v.size(); ++i) {
    a += v[i];
  }
  return a;
}
