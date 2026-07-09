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
#include <stdexcept>
#include <vector>

class ThetaErrorTermDenominators {
 private:
  long double (*denominatorFunction)(uint64_t);

  // different denominator functions
  static long double tripleLogDenom(uint64_t x);
  static long double singleLogDenom(uint64_t x);
  static long double sqrtDenom(uint64_t x);
  static long double powersOfKDenom(uint64_t x);

 public:
  ThetaErrorTermDenominators() = default;
  long double computeDenominator(uint64_t x) { return denominatorFunction(x); }
  void setDenominator(int whichDenominator);
};

extern ThetaErrorTermDenominators denominator;
