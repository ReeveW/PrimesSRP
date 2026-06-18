#include "PrimeCharacteristics.hpp"

void computeAll(const uint64_t upperBoundOfN, const uint64_t x) {
  std::ofstream* out = new std::ofstream("error_data.csv");
  for (int i = 2; i < upperBoundOfN; ++i) {
    eTheta(i, x, out);
  }
  delete out;
}

struct ThreadData {
  uint64_t x, n;
  std::ostream* output;
};

void* eThetaThread(void* arg) {
  ThreadData* data = static_cast<ThreadData*>(arg);

  eTheta(data->n, data->x, data->output);

  return nullptr;
}

// works wierdly if upperBoundOfN is not divisible by threadCount
void computeAllWithMultiThreading(const uint64_t upperBoundOfN,
                                  const uint64_t x, int threadCount,
                                  std::vector<std::ostream*> outputFiles) {
  uint64_t iterations = upperBoundOfN / threadCount;
  std::vector<pthread_t> v(threadCount);
  std::vector<ThreadData> threadData(threadCount);
  for (int i = 0; i < iterations; ++i) {
    for (int j = 0; j < v.size(); ++j) {
      uint64_t n = i + (j * iterations);
      if (n == 0 || n == 1) continue;
      threadData[j] = {n, x, outputFiles[j]};
      pthread_create(&v[j], nullptr, eThetaThread, &threadData[j]);
    }
    for (auto& x : v) {
      pthread_join(x, nullptr);
    }
  }
  return;
}

void eTheta(const uint64_t n, const uint64_t x, std::ostream* out) {
  std::vector<uint64_t> cutoffs = {10,          100,          1000,
                                   10000,       100000,       1000000,
                                   10000000,    100000000,    1000000000,
                                   10000000000, 100000000000, 1000000000000};
  int currentCutoff = 0;
  primesieve::iterator it;
  uint64_t prime;
  ThetaErrorInfo t(n, cutoffs.size());
  uint64_t phin = phi(n);

  outputHeaderForN(n, out);

  while ((prime = it.next_prime()) < x) {
    if (prime > cutoffs[currentCutoff]) {
      nextCutoff(cutoffs, currentCutoff, n, t, phin, out);
    }
    uint64_t a = prime % n;
    updateErrorTerms(t, prime, phin, n, a);

    // code for first prime in AP, max gap in each cutoff
    if (!t.firstPrimeInAP[a]) {
      t.firstPrimeInAP[a] = prime;
    } else {
      t.largestGapInAP[a] =
          std::max(t.largestGapInAP[a], prime - t.lastPrimeInAP[a]);
    }
    t.lastPrimeInAP[a] = prime;
  }
  return;
}

void outputHeaderForN(uint64_t n, std::ostream* out) {
  *out << "n = " << n << std::endl
       << "current cutoff, a, max error, prime of "
          "max error, min error, prime of min error"
       << std::endl;
  return;
}

void updateErrorTerms(ThetaErrorInfo& t, uint64_t prime, uint64_t phin,
                      uint64_t n, uint64_t a) {
  long double d = denom(prime);
  long double num = numerator(phin, prime);

  long double currentMin = error(t.thetaInAP[a], num, d);
  if (currentMin < t.minError[a]) {
    t.minError[a] = currentMin;
    t.primeOfMinError[a] = prime;
  }
  t.thetaInAP[a] += std::log(static_cast<long double>(prime));
  long double currentMax = error(t.thetaInAP[a], num, d);
  if (currentMax > t.maxError[a]) {
    t.maxError[a] = currentMax;
    t.primeOfMaxError[a] = prime;
  }

  return;
}

long double denom(uint64_t x) {
  return std::sqrt(x) *
         std::pow(std::log(std::log(std::log(static_cast<long double>(x)))), 2);
}

long double numerator(uint64_t phin, uint64_t x) { return (1.0L / phin) * x; }

// now no longer stops when we go past the last cutoff, is that a problem ever?
void nextCutoff(std::vector<uint64_t>& cutoffs, int& currentCutoff, uint64_t n,
                ThetaErrorInfo& t, uint64_t phin, std::ostream* out) {
  uint64_t x = cutoffs[currentCutoff];
  long double d = denom(x);
  long double num = numerator(phin, x);
  for (uint64_t i = 0; i < n; ++i) {
    if (std::gcd(i, n) != 1) {
      continue;
    }
    long double e = error(t.thetaInAP[i], num, d);

    updateCutoffErrors(e, t, i, x, currentCutoff);

    outputErrorDataForCutoff(x, i, t, out);

    resetErrorForCutoff(e, t, i, x);
  }
  currentCutoff++;
  return;
}

void outputErrorDataForCutoff(uint64_t cutoff, uint64_t a,
                              const ThetaErrorInfo& t, std::ostream* out) {
  *out << std::scientific << std::setprecision(0)
       << static_cast<long double>(cutoff) << "," << std::defaultfloat << a
       << "," << std::fixed << std::setprecision(10) << t.maxError[a] << ","
       << t.primeOfMaxError[a] << "," << t.minError[a] << ","
       << t.primeOfMinError[a] << std::endl;
  return;
}

void resetErrorForCutoff(long double e, ThetaErrorInfo& t, uint64_t i,
                         uint64_t x) {
  t.maxError[i] = e;
  t.primeOfMaxError[i] = x;
  t.minError[i] = e;
  t.primeOfMinError[i] = x;
  t.largestGapInAP[i] = 0;
  return;
}

void updateCutoffErrors(long double e, ThetaErrorInfo& t, uint64_t i,
                        uint64_t x, int currentCutoff) {
  if (e < t.minError[i]) {
    t.minError[i] = e;
    t.primeOfMinError[i] = x;
  }
  if (e > t.maxError[i]) {
    t.maxError[i] = e;
    t.primeOfMaxError[i] = x;
  }

  if (t.maxError[i] > t.maxOutliers[currentCutoff].error) {
    t.maxOutliers[currentCutoff] = {i, t.primeOfMaxError[i], t.maxError[i]};
  }
  if (t.minError[i] < t.minOutliers[currentCutoff].error) {
    t.minOutliers[currentCutoff] = {i, t.primeOfMinError[i], t.minError[i]};
  }
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

int add(const std::vector<uint64_t>& v) {
  int a = 0;
  for (int i = 0; i < v.size(); ++i) {
    a += v[i];
  }
  return a;
}
