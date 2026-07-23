#include "PrimeCharacteristics.hpp"

void computeAllThread(uint64_t start, uint64_t end, uint64_t increment,
                      uint64_t x, std::ostream* out,
                      std::ostream* maxOverAOutput,
                      std::ostream* largestGapOutput,
                      std::ostream* firstPrimeOutput) {
  for (uint64_t n = start; n <= end; n += increment) {
    eTheta(n, x, out, maxOverAOutput, largestGapOutput, firstPrimeOutput);
  }
  return;
}

struct ThreadData {
  uint64_t start, x, end, increment;
  std::ostream *output, *maxOverAOutput, *largestGapOutput, *firstPrimeOutput;
};

void* eThetaThread(void* arg) {
  ThreadData* data = static_cast<ThreadData*>(arg);

  computeAllThread(data->start, data->end, data->increment, data->x,
                   data->output, data->maxOverAOutput, data->largestGapOutput,
                   data->firstPrimeOutput);

  return nullptr;
}

void computeAllWithMultiThreading(
    const uint64_t upperBoundOfN, const uint64_t x, uint64_t threadCount,
    bool primePowers, std::vector<std::ofstream>& outputFiles,
    std::vector<std::ofstream>& maxOverAOutputFiles,
    std::vector<std::ofstream>& largestGapFile,
    std::vector<std::ofstream>& firstPrimeFile, int whichDenominator) {
  std::vector<pthread_t> threads(threadCount);
  std::vector<ThreadData> data(threadCount);

  PreComputedPrimeIterator p = PreComputedPrimeIterator(x, primePowers);

  denominator.setDenominator(whichDenominator);

  uint64_t chunk = upperBoundOfN / threadCount;
  uint64_t end = upperBoundOfN;
  for (uint64_t j = 0; j < threadCount; ++j) {
    uint64_t start = j;
    if (start < 2) {
      start += threadCount;
    }
    data[j] = {start,
               x,
               end,
               threadCount,
               &outputFiles[j],
               &maxOverAOutputFiles[j],
               &largestGapFile[j],
               &firstPrimeFile[j]};

    pthread_create(&threads[j], nullptr, eThetaThread, &data[j]);
  }

  for (auto& t : threads) {
    pthread_join(t, nullptr);
  }
}

void eTheta(const uint64_t n, const uint64_t x, std::ostream* out,
            std::ostream* maxOverAOutput, std::ostream* largestGapOutput,
            std::ostream* firstPrimeOutput) {
  std::vector<uint64_t> cutoffs = {10,          100,          1000,
                                   10000,       100000,       1000000,
                                   10000000,    100000000,    1000000000,
                                   10000000000, 100000000000, 1000000000000};
  int currentCutoff = 0;
  PreComputedPrimeIterator p = PreComputedPrimeIterator();
  uint64_t prime;
  ThetaErrorInfo t(n, cutoffs.size());
  uint64_t phin = phi(n);

#if defined(THETA_DATA)
  outputHeaderForN(n, out);
#endif

  *maxOverAOutput << "n = " << n << std::endl;

  while ((prime = p.next_prime()) < x) {
    if (prime > cutoffs[currentCutoff]) {
      nextCutoff(cutoffs, currentCutoff, n, t, phin, out, maxOverAOutput,
                 largestGapOutput, firstPrimeOutput);
    }
    uint64_t a = prime % n;
#if defined(THETA_DATA)
    updateErrorTerms(t, prime, phin, n, a);
#endif

#if defined(FIRST_PRIME)
    if (!t.firstPrimeInAP[a]) {
      t.firstPrimeInAP[a] = prime;
    }
#endif
#if defined(LARGEST_GAP)
    if (t.lastPrimeInAP[a]) {
      t.largestGapInAP[a] =
          std::max(t.largestGapInAP[a], prime - t.lastPrimeInAP[a]);
    }
#endif

    t.lastPrimeInAP[a] = prime;
  }

  // make it so that once we hit x, we treat it as another cutoff and output
  // error terms
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
  long double d = denominator.computeDenominator(prime);
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

long double numerator(uint64_t phin, uint64_t x) { return (1.0L / phin) * x; }

// now no longer stops when we go past the last cutoff, is that a problem ever?
void nextCutoff(std::vector<uint64_t>& cutoffs, int& currentCutoff, uint64_t n,
                ThetaErrorInfo& t, uint64_t phin, std::ostream* out,
                std::ostream* maxOverAOutput, std::ostream* largestGapOutput,
                std::ostream* firstPrimeOutput) {
  uint64_t x = cutoffs[currentCutoff];
  #if defined(THETA_DATA)
  long double d = denominator.computeDenominator(x);
  long double num = numerator(phin, x);

  allAErrorData allA;
  #endif

  for (uint64_t a = 0; a < n; ++a) {
    if (std::gcd(a, n) != 1) {
      continue;
    }
#if defined(THETA_DATA)
    long double e = error(t.thetaInAP[a], num, d);

    updateCutoffErrors(e, t, a, x, currentCutoff);

    if (allA.minOverAllA > t.minError[a]) {
      allA.minOverAllA = t.minError[a];
      allA.aMin = a;
      allA.aMinPrime = t.primeOfMinError[a];
    }
    if (allA.maxOverAllA < t.maxError[a]) {
      allA.maxOverAllA = t.maxError[a];
      allA.aMax = a;
      allA.aMaxPrime = t.primeOfMaxError[a];
    }
    outputErrorDataForCutoff(x, a, t, out);
#endif

#if defined(FIRST_PRIME)
    outputFirstPrimeForCutoff(x, a, t, firstPrimeOutput);
#endif
#if defined(LARGEST_GAP)
    outputLargestGapForCutoff(x, a, t, largestGapOutput);
#endif

#if defined(THETA_DATA)
    resetErrorForCutoff(e, t, a, x);
#endif
#if defined(LARGEST_GAP)
    t.largestGapInAP[a] = 0;
#endif
  }

  #if defined(THETA_DATA)
  outputErrorOverAllA(x, allA, maxOverAOutput);
  #endif

  currentCutoff++;
  return;
}

void outputFirstPrimeForCutoff(uint64_t cutoff, uint64_t a,
                               const ThetaErrorInfo& t, std::ostream* out) {
  *out << std::scientific << std::setprecision(0)
       << static_cast<long double>(cutoff) << "," << std::defaultfloat << a
       << "," << std::defaultfloat << t.firstPrimeInAP[a] << std::endl;
  return;
}

void outputLargestGapForCutoff(uint64_t cutoff, uint64_t a,
                               const ThetaErrorInfo& t, std::ostream* out) {
  *out << std::scientific << std::setprecision(0)
       << static_cast<long double>(cutoff) << "," << std::defaultfloat << a
       << "," << std::defaultfloat << t.largestGapInAP[a] << std::endl;
  return;
}

void outputErrorOverAllA(uint64_t cutoff, allAErrorData allA,
                         std::ostream* out) {
  *out << std::scientific << std::setprecision(0)
       << static_cast<long double>(cutoff) << "," << std::defaultfloat
       << allA.aMax << "," << std::defaultfloat << allA.aMaxPrime << ","
       << std::fixed << std::setprecision(ERROR_DECIMAL_PRECISION)
       << allA.maxOverAllA << "," << std::defaultfloat << allA.aMin << ","
       << std::defaultfloat << allA.aMinPrime << "," << allA.minOverAllA
       << std::endl;
}

void outputErrorDataForCutoff(uint64_t cutoff, uint64_t a,
                              const ThetaErrorInfo& t, std::ostream* out) {
  *out << std::scientific << std::setprecision(0)
       << static_cast<long double>(cutoff) << "," << std::defaultfloat << a
       << "," << std::fixed << std::setprecision(ERROR_DECIMAL_PRECISION)
       << t.maxError[a] << "," << t.primeOfMaxError[a] << "," << t.minError[a]
       << "," << t.primeOfMinError[a] << std::endl;
  return;
}

void resetErrorForCutoff(long double e, ThetaErrorInfo& t, uint64_t i,
                         uint64_t x) {
  t.maxError[i] = e;
  t.primeOfMaxError[i] = x;
  t.minError[i] = e;
  t.primeOfMinError[i] = x;
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
#if defined(THETA_DATA)
  for (uint64_t a = 1; a < n; ++a) {
    if (std::gcd(a, n) == 1) {
      count++;
    }
  }
#endif
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
