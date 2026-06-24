#include <gmp.h>
#include <pthread.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <primesieve.hpp>
#include <vector>

#include "PrimeCharacteristics.hpp"
#include "PrimeGeneration.hpp"

int main() {
  const auto start{std::chrono::steady_clock::now()};

  std::ofstream file0("thread0.csv");
  std::ofstream file1("thread1.csv");
  std::ofstream file2("thread2.csv");
  std::ofstream file3("thread3.csv");

  std::vector<std::ostream*> outputs = {&file0, &file1, &file2, &file3};

  std::ofstream afile0("Athread0.csv");
  std::ofstream afile1("Athread1.csv");
  std::ofstream afile2("Athread2.csv");
  std::ofstream afile3("Athread3.csv");

  std::vector<std::ostream*> aOutputs = {&afile0, &afile1, &afile2, &afile3};

  computeAllWithMultiThreading(100, 1e7, 4, false, outputs, aOutputs);

  const auto finish{std::chrono::steady_clock::now()};
  const std::chrono::duration<double> elapsed_seconds{finish - start};
  std::cout << "time to compute: " << elapsed_seconds.count() << std::endl;

  // PreComputedPrimeIterator p = PreComputedPrimeIterator(1000, true);
  // uint64_t x;
  // while((x = p.next_prime()) < 1000){
  //   std::cout << x << " ";
  // }
  // std::cout << std::endl;

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