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

std::vector<long double> eTheta(const uint64_t n, const uint64_t x) {
  std::vector<uint64_t> cutoffs = {10,         100,        1000,     10000,
                                   100000,     1000000,    10000000, 100000000,
                                   1000000000, 10000000000};
  int currentCutoff = 0;
  primesieve::iterator it;
  uint64_t prime;

  std::vector<uint64_t> lastPrimeInAP(n, 0);
  std::vector<long double> thetaInAP(n, 0);
  std::vector<long double> maxError(n, 0);
  std::vector<long double> minError(
      n, std::numeric_limits<long double>::infinity());
  std::vector<OutlierInfo> maxOutliers(10, {0, 0, 0});
  std::vector<OutlierInfo> minOutliers(
      10, {0, 0, std::numeric_limits<long double>::infinity()});

  long double denom =
      std::sqrt(10) * std::log(std::log(static_cast<long double>(10)));
  long double numerator = 1.0L / phi(10) * 10;

  while ((prime = it.next_prime()) < x) {
    if (prime > cutoffs[currentCutoff]) {
      nextCutoff(maxError, minError, cutoffs, currentCutoff, thetaInAP,
                 maxOutliers, minOutliers, n, denom, numerator);
      if (currentCutoff + 1 < cutoffs.size()) {
        currentCutoff++;
      } else {
        return thetaInAP;
      }
    }
    uint64_t a = prime % n;
    if (lastPrimeInAP[a] != 0) {
      maxError[a] =
          std::max(maxError[a], error(thetaInAP[a], numerator, denom));
    }
    lastPrimeInAP[a] = prime;
    thetaInAP[a] += std::log(static_cast<long double>(prime));
    minError[a] = std::min(minError[a], error(thetaInAP[a], numerator, denom));
  }
  return thetaInAP;
}

void nextCutoff(std::vector<long double>& maxError,
                std::vector<long double>& minError,
                std::vector<uint64_t>& cutoffs, int currentCutoff,
                std::vector<long double>& thetaInAP,
                std::vector<OutlierInfo>& maxOutliers,
                std::vector<OutlierInfo>& minOutliers, uint64_t n,
                long double& denom, long double& numerator) {
  int x = cutoffs[currentCutoff];
  for (uint64_t i = 0; i < n; ++i) {
    maxError[i] = std::max(maxError[i], error(thetaInAP[i], numerator, denom));
    if (maxError[i] > maxOutliers[currentCutoff].error) {
      maxOutliers[currentCutoff] = {i, n, maxError[i]};
    }
    if (minError[i] < minOutliers[currentCutoff].error) {
      minOutliers[currentCutoff] = {i, n, minError[i]};
    }
    std::cout << "cutoff = " << x << " residue = " << i
              << " theta = " << thetaInAP[i] << " error = " << maxError[i]
              << " min = " << minError[i] << '\n';
  }

  denom = std::sqrt(x) * std::log(std::log(static_cast<long double>(x)));
  numerator = 1.0L / phi(x) * x;
  return;
}

// should this use the cutoff (x) or n?
uint64_t phi(const uint64_t cutoff) {
  uint64_t count = 0;
  for (uint64_t a = 1; a < cutoff; ++a) {
    if (std::gcd(a, cutoff) == 1) {
      count++;
    }
  }
  return count;
}

long double error(long double thetaOfA, long double numerator,
                  long double denom) {
  return (thetaOfA - numerator) / denom;
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
