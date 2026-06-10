#include <gmp.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <primesieve.hpp>
#include <vector>

#include "PrimeCharacteristics.hpp"

int main() {
  std::vector<long double> theta = eTheta(4, 1000000100);
  std::cout << theta[1] << std::endl << theta[3] << std::endl;

  return 0;
}
/*
cutoff = 10 residue = 0 theta = 0 max = -48.0063 found at: 10 min = -48.0063 found at: 10 e: -48.0063
cutoff = 10 residue = 1 theta = 1.60944 max = -0.72226 found at: 5 min = -32.5537 found at: 10 e: -32.5537
cutoff = 10 residue = 2 theta = 0.693147 max = -41.3512 found at: 10 min = -41.3512 found at: 10 e: -41.3512
cutoff = 10 residue = 3 theta = 3.04452 max = -0.0414693 found at: 3 min = -18.775 found at: 10 e: -18.775
cutoff = 100 residue = 0 theta = 0 max = -27.8883 found at: 100 min = -48.0063 found at: 10 e: -27.8883
cutoff = 100 residue = 1 theta = 39.1344 max = -5.04267 found at: 61 min = -637.181 found at: 17 e: -6.06048
cutoff = 100 residue = 2 theta = 0.693147 max = -27.5017 found at: 100 min = -41.3512 found at: 10 e: -27.5017
cutoff = 100 residue = 3 theta = 43.9009 max = 1.68047 found at: 83 min = -157.472 found at: 19 e: -3.40188
53.1682
53.2084
*/