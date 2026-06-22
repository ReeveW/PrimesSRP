#include <gmp.h>
#include <pthread.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <primesieve.hpp>
#include <vector>

#include "PrimeCharacteristics.hpp"

int main() {
  const auto start{std::chrono::steady_clock::now()};

  std::ofstream file0("thread0.csv");
  std::ofstream file1("thread1.csv");
  std::ofstream file2("thread2.csv");
  std::ofstream file3("thread3.csv");

  std::vector<std::ostream*> outputs = {&file0, &file1, &file2, &file3};

  computeAllWithMultiThreading(123, 1e8, 4, outputs);

  // eTheta(4, 1000, &file0);

  const auto finish{std::chrono::steady_clock::now()};
  const std::chrono::duration<double> elapsed_seconds{finish - start};
  std::cout << "time to compute: " << elapsed_seconds.count() << std::endl;

  return 0;
}
/*
time to compute: 1.6057 for n = 4
time to compute: 1.61337 for n = 12
time to compute: 1.61579 for n = 23
time to compute: 1.60566 for n = 100
time to compute: 1.59321 for n = 1000
time to compute: 1.69854 for n = 73218
time to compute: 4.77267 for n = 3921093
time to compute: 7.89275 for n = 10000000
*/