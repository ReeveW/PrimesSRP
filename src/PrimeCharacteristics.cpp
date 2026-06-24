#include "PrimeCharacteristics.hpp"

void computeAll(const uint64_t upperBoundOfN, const uint64_t x) {
  std::ofstream* out = new std::ofstream("error_data.csv");
  std::ofstream* a = new std::ofstream("maxA.csv");
  for (int i = 2; i < upperBoundOfN; ++i) {
    eTheta(i, x, out, a);
  }
  delete out;
}

void computeAllThread(uint64_t start, uint64_t end, uint64_t increment,
                      uint64_t x, std::ostream* out,
                      std::ostream* maxOverAOutput) {
  for (uint64_t n = start; n <= end; n += increment) {
    eTheta(n, x, out, maxOverAOutput);
  }
  return;
}

struct ThreadData {
  uint64_t start, x, end, increment;
  std::ostream *output, *maxOverAOutput;
};

void* eThetaThread(void* arg) {
  ThreadData* data = static_cast<ThreadData*>(arg);

  computeAllThread(data->start, data->end, data->increment, data->x,
                   data->output, data->maxOverAOutput);

  return nullptr;
}

void computeAllWithMultiThreading(
    const uint64_t upperBoundOfN, const uint64_t x, uint64_t threadCount,
    bool primePowers, std::vector<std::ostream*> outputFiles,
    std::vector<std::ostream*> maxOverAOutputFiles) {
  std::vector<pthread_t> threads(threadCount);
  std::vector<ThreadData> data(threadCount);

  PreComputedPrimeIterator p = PreComputedPrimeIterator(x, primePowers);

  uint64_t chunk = upperBoundOfN / threadCount;
  uint64_t end = upperBoundOfN;
  for (uint64_t j = 0; j < threadCount; ++j) {
    uint64_t start = j;
    if (start < 2) {
      start += threadCount;
    }
    data[j] = {
        start, x, end, threadCount, outputFiles[j], maxOverAOutputFiles[j]};

    pthread_create(&threads[j], nullptr, eThetaThread, &data[j]);
  }

  for (auto& t : threads) {
    pthread_join(t, nullptr);
  }
}

void eTheta(const uint64_t n, const uint64_t x, std::ostream* out,
            std::ostream* maxOverAOutput) {
  std::vector<uint64_t> cutoffs = {10,          100,          1000,
                                   10000,       100000,       1000000,
                                   10000000,    100000000,    1000000000,
                                   10000000000, 100000000000, 1000000000000};
  int currentCutoff = 0;
  PreComputedPrimeIterator p = PreComputedPrimeIterator();
  uint64_t prime;
  ThetaErrorInfo t(n, cutoffs.size());
  uint64_t phin = phi(n);

  outputHeaderForN(n, out);

  *maxOverAOutput << "n = " << n << std::endl;

  while ((prime = p.next_prime()) < x) {
    if (prime > cutoffs[currentCutoff]) {
      nextCutoff(cutoffs, currentCutoff, n, t, phin, out, maxOverAOutput);
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
                ThetaErrorInfo& t, uint64_t phin, std::ostream* out,
                std::ostream* maxOverAOutput) {
  uint64_t x = cutoffs[currentCutoff];
  long double d = denom(x);
  long double num = numerator(phin, x);

  long double minOverAllA = 10000;
  uint64_t aMin;
  uint64_t aMinPrime;
  long double maxOverAllA = -10000;
  uint64_t aMax;
  uint64_t aMaxPrime;

  for (uint64_t a = 0; a < n; ++a) {
    if (std::gcd(a, n) != 1) {
      continue;
    }
    long double e = error(t.thetaInAP[a], num, d);

    updateCutoffErrors(e, t, a, x, currentCutoff);

    if (minOverAllA > t.minError[a]) {
      minOverAllA = t.minError[a];
      aMin = a;
      aMinPrime = t.primeOfMinError[a];
    }
    if (maxOverAllA < t.maxError[a]) {
      maxOverAllA = t.maxError[a];
      aMax = a;
      aMaxPrime = t.primeOfMaxError[a];
    }

    outputErrorDataForCutoff(x, a, t, out);

    resetErrorForCutoff(e, t, a, x);
  }

  *maxOverAOutput << minOverAllA << "," << aMin << "," << aMinPrime << ","
                  << maxOverAllA << "," << aMax << "," << aMaxPrime
                  << std::endl;

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
