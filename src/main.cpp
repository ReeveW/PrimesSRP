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
