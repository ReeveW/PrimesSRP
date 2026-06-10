#pragma once
#include <gmp.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>
#include <primesieve.hpp>
#include <unordered_map>
#include <vector>

struct OutlierInfo {
  uint64_t a;
  uint64_t n;
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
                    {0, 0, std::numeric_limits<long double>::infinity()}) {}
};

/*
finds the largest gap between two primes, given a list of primes.
If the list has <= 1 elements then it returns 0.
This is purely to get a feel for things like primesieve and time benchmarks.
*/

uint64_t largestGap(const std::vector<uint64_t>& primes);

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

/*

*/

std::vector<long double> eTheta(const uint64_t n, const uint64_t x);

long double denom(uint64_t x);

long double numerator(uint64_t phin, uint64_t x);

void nextCutoff(std::vector<uint64_t>& cutoffs, int& currentCutoff,
                 uint64_t n, ThetaErrorInfo& t,
                long double denom, long double numerator);

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
computes the primes mod n for all n less than upperBound, with x as the upper
limit of primes we look for.
*/

void computeAll(const uint64_t upperBound, const uint64_t x);

/*
adds up all the values in a vector. Used to verify that the number of primes
adds up when testing function primesModN.
*/

int add(const std::vector<uint64_t>& v);
