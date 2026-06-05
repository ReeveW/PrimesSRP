#include <gmp.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <primesieve.hpp>
#include <vector>

#include "PrimeCharacteristics.hpp"

int main() {
  std::vector<long double> theta = eTheta(4, 120);
  std::cout << theta[1] << std::endl << theta[3] << std::endl;

  return 0;
}
/*
outputs the following on eTheta(4, 120):
cutoff = 10 residue = 0 theta = 0 max = -48.0063 min = inf e: -48.0063
cutoff = 10 residue = 1 theta = 1.60944 max = -0.798742 min = -0.72226 e: -32.5537
cutoff = 10 residue = 2 theta = 0.693147 max = -41.3512 min = inf e: -41.3512
cutoff = 10 residue = 3 theta = 3.04452 max = -2.66962 min = -1.03993 e: -18.775
cutoff = 100 residue = 0 theta = 0 max = -27.8883 min = inf e: -27.8883
cutoff = 100 residue = 1 theta = 39.1344 max = -0.798742 min = -219.837 e: -6.06048
cutoff = 100 residue = 2 theta = 0.693147 max = -27.5017 min = inf e: -27.5017
cutoff = 100 residue = 3 theta = 43.9009 max = -1.07899 min = -43.2003 e: -3.40188
53.1682
53.2084

I verified the e values by hand and everything lines up which is good. should verify the max and min values,
will need to know the primes that gave those values, but that should be included when collecting data anyways.
*/