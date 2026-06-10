#include "PrimeCharacteristics.hpp"

std::vector<long double> eTheta(const uint64_t n, const uint64_t x) {
  std::vector<uint64_t> cutoffs = {10,          100,          1000,
                                   10000,       100000,       1000000,
                                   10000000,    100000000,    1000000000,
                                   10000000000, 100000000000, 1000000000000};
  int currentCutoff = 0;
  primesieve::iterator it;
  uint64_t prime;
  ThetaErrorInfo t(n, cutoffs.size());
  uint64_t phin = phi(n);

  while ((prime = it.next_prime()) < x) {
    if (prime > cutoffs[currentCutoff]) {
      long double d = denom(cutoffs[currentCutoff]);
      long double num = numerator(phin, cutoffs[currentCutoff]);
      nextCutoff(cutoffs, currentCutoff, n, t, d, num);
    }
    
    long double d = denom(prime);
    long double num = numerator(phin, prime);
    uint64_t a = prime % n;
    int currentMin = error(t.thetaInAP[a], num, d);
    if (currentMin > t.minError[a]) {
      t.minError[a] = currentMin;
      t.primeOfMinError[a] = prime;
    }
    t.lastPrimeInAP[a] = prime;
    t.thetaInAP[a] += std::log(static_cast<long double>(prime));
    uint64_t currentMax = error(t.thetaInAP[a], num, d);
    if (currentMax > t.maxError[a]) {
      t.maxError[a] = currentMax;
      t.primeOfMaxError[a] = prime;
    }
  }
  return t.thetaInAP;
}

long double denom(uint64_t x) {
  return std::sqrt(x) *
         std::pow(std::log(std::log(std::log(static_cast<long double>(x)))), 2);
}

long double numerator(uint64_t phin, uint64_t x) { return (1.0L / phin) * x; }

// now no longer stops when we go past the last cutoff, is that a problem ever?
void nextCutoff(std::vector<uint64_t>& cutoffs, int& currentCutoff, uint64_t n,
                ThetaErrorInfo& t, long double d, long double num) {
  uint64_t x = cutoffs[currentCutoff];
  for (uint64_t i = 0; i < n; ++i) {
    long double e = error(t.thetaInAP[i], num, d);
    t.maxError[i] = std::max(t.maxError[i], e);
    if (t.maxError[i] > t.maxOutliers[currentCutoff].error) {
      t.maxOutliers[currentCutoff] = {i, n, t.maxError[i]};
    }
    if (t.minError[i] < t.minOutliers[currentCutoff].error) {
      t.minOutliers[currentCutoff] = {i, n, t.minError[i]};
    }
    // temporary output statements
    std::cout << "cutoff = " << x << " residue = " << i
              << " theta = " << t.thetaInAP[i] << " max = " << t.maxError[i]
              << " min = " << t.minError[i] << " e: " << e << '\n';
  }
  currentCutoff++;
  return;
}

uint64_t phi(const uint64_t n) {
  uint64_t count = 0;
  for (uint64_t a = 1; a < n; ++a) {
    if (std::gcd(a, n) == 1) {
      count++;
    }
  }
  return count;
}

long double error(long double thetaOfA, long double numerator,
                  long double denom) {
  return (thetaOfA - numerator) / denom;
}



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
  std::vector<uint64_t> largestGapInAP(n, 0);  // tracks largest number of steps
  std::vector<uint64_t> primeCountInAP(n, 0);
  std::vector<uint64_t> lastPrimeInAP(n, 0);

  while ((prime = it.next_prime()) < x) {
    uint64_t a = prime % n;
    if (primeCountInAP[a] == 0) {
      firstPrimeInAP[a] = prime;
    } else {
      largestGapInAP[a] =
          std::max(largestGapInAP[a], ((prime / n) - lastPrimeInAP[a]));
    }
    primeCountInAP[a]++;
    lastPrimeInAP[a] = prime / n;
  }
  return primeCountInAP;
}

void computeAll(const uint64_t upperBound, const uint64_t x) {
  for (int i = 2; i < upperBound; ++i) {
    primesModN(i, x);  // how should i be storing this information now?
  }
}

int add(const std::vector<uint64_t>& v) {
  int a = 0;
  for (int i = 0; i < v.size(); ++i) {
    a += v[i];
  }
  return a;
}
