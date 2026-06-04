#include <gmp.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <primesieve.hpp>
#include <vector>

#include "PrimeCharacteristics.hpp"

int main() {
  std::vector<long double> theta = eTheta(4, 100);
  std::cout << theta[1] << std::endl << theta[3] << std::endl;

  return 0;
}
/*
outputs the following on eTheta(4, 120): 
cutoff = 10 residue = 0 theta = 0 error = 0 min = inf
cutoff = 10 residue = 1 theta = 1.60944 error = 0 min = -0.337661
cutoff = 10 residue = 2 theta = 0.693147 error = 0 min = -0.685078
cutoff = 10 residue = 3 theta = 3.04452 error = 0.206459 min = -0.531343
cutoff = 100 residue = 0 theta = 0 error = 0 min = inf
cutoff = 100 residue = 1 theta = 39.1344 error = 13.8901 min = -0.337661
cutoff = 100 residue = 2 theta = 0.693147 error = 0 min = -0.685078
cutoff = 100 residue = 3 theta = 43.9009 error = 15.6974 min = -0.531343
53.1682
53.2084
*/