#pragma once

#include <PrimeGeneration.hpp>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <primesieve.hpp>
#include <vector>

const int ERROR_DECIMAL_PRECISION = 30;

struct OutlierInfo {
  uint64_t a;
  uint64_t p;
  long double error;
};

struct ThetaErrorInfo {
  std::vector<uint64_t> lastPrimeInAP;
  std::vector<long double> thetaInAP;
  std::vector<long double> maxError;
  std::vector<long double> minError;
  std::vector<uint64_t> primeOfMaxError;
  std::vector<uint64_t> primeOfMinError;
  std::vector<OutlierInfo> maxOutliers;
  std::vector<OutlierInfo> minOutliers;
  std::vector<uint64_t> cutoffs;
  std::vector<uint64_t> firstPrimeInAP;
  std::vector<uint64_t> largestGapInAP;

  ThetaErrorInfo(uint64_t n, int numberOfCutoffs)
      : lastPrimeInAP(n, 0),
        thetaInAP(n, 0),
        maxError(n, -std::numeric_limits<long double>::infinity()),
        minError(n, std::numeric_limits<long double>::infinity()),
        primeOfMaxError(n),
        primeOfMinError(n),
        maxOutliers(numberOfCutoffs,
                    {0, 0, -std::numeric_limits<long double>::infinity()}),
        minOutliers(numberOfCutoffs,
                    {0, 0, std::numeric_limits<long double>::infinity()}),
        firstPrimeInAP(n, 0),
        largestGapInAP(n, 0) {}
};

struct allAErrorData {
  long double minOverAllA = 10000;
  uint64_t aMin;
  uint64_t aMinPrime;
  long double maxOverAllA = -10000;
  uint64_t aMax;
  uint64_t aMaxPrime;
};

// make it a default constructor, then have a setter which takes an int and
// turns it into the denom function
// move this to a seperate file along with the denom functions
class ThetaErrorTermDenominators {
 private:
  long double (*denominatorFunction)(uint64_t);

 public:
  ThetaErrorTermDenominators(long double (*f)(uint64_t))
      : denominatorFunction(f) {}
  long double computeDenominator(uint64_t x) { return denominatorFunction(x); }
};

std::unique_ptr<ThetaErrorTermDenominators> computeDenom;

/*
computes the primes mod n for all n less than upperBound, with x as the upper
limit of primes we look for.
*/

void computeAll(const uint64_t upperBoundOfN, const uint64_t x);

void* eThetaThread(void* arg);

void computeAllThread(uint64_t start, uint64_t end, uint64_t increment,
                      uint64_t x, std::ostream* out,
                      std::ostream* maxOverAOutput);

void computeAllWithMultiThreading(
    const uint64_t upperBoundOfN, const uint64_t x, uint64_t threadCount,
    bool primePowers, std::vector<std::ofstream>& outputFiles,
    std::vector<std::ofstream>& maxOverAOutputFiles, int whichDenominator);

/*

*/

void eTheta(const uint64_t n, const uint64_t x, std::ostream* out,
            std::ostream* maxOverAOutput);

void outputHeaderForN(uint64_t n, std::ostream* out);

void updateErrorTerms(ThetaErrorInfo& t, uint64_t prime, uint64_t phin,
                      uint64_t n, uint64_t a);

void resetErrorForCutoff(long double e, ThetaErrorInfo& t, uint64_t i,
                         uint64_t x);

void updateCutoffErrors(long double e, ThetaErrorInfo& t, uint64_t i,
                        uint64_t x, int currentCutoff);

long double tripleLogDenom(uint64_t x);

long double numerator(uint64_t phin, uint64_t x);

void nextCutoff(std::vector<uint64_t>& cutoffs, int& currentCutoff, uint64_t n,
                ThetaErrorInfo& t, uint64_t phin, std::ostream* out,
                std::ostream* maxOverAOutput);

void outputErrorOverAllA(uint64_t cutoff, allAErrorData allA,
                         std::ostream* out);

void outputErrorDataForCutoff(uint64_t cutoff, uint64_t a,
                              const ThetaErrorInfo& t, std::ostream* out);

long double error(long double thetaOfA, long double numerator,
                  long double denom);

/*
calculates Euler's totient function for n.
for all numbers a in the range of 1 to n-1, finds the gcd of a and n, if
it's 1 we increment the count. returns the count of numbers relatively prime
to n.
*/

uint64_t phi(const uint64_t n);

/*
Computes the number of primes in each arithmetic progression
Inputs: x - the upper limit of primes we look for.
        n - the modulus with which we use to find a.
For each prime (p) < x, this function updates the characteristics of a (p mod
n), such as incrementing count[a].
time complexity: O(pi(x)), as the operations we perform in the while loop take
constant time. returns a vector of size n, which is the number of arithmetic
progressions we use.
*/

std::vector<uint64_t> primesModN(const uint64_t n, const uint64_t x);
