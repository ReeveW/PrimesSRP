#include <gmp.h>
#include <pthread.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <primesieve.hpp>
#include <vector>

#include "PrimeCharacteristics.hpp"
#include "PrimeGeneration.hpp"

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cout << "Usage: ./main <x> <n> <threads> --commands (find in readme)\n";
    return 1;
  }
  uint64_t x = std::stoull(argv[1]);
  uint64_t n = std::stoull(argv[2]);
  int threads = std::stoi(argv[3]);

  bool primePowers = false;
  int whichDenom = 0;
  for(int i = 4; i < argc; ++i){
    std::string arg = argv[i];
    if(arg == "--primePowers"){
      primePowers = true;
    }
    if(arg == "--sqrt"){
      whichDenom = 1;
    }
  }

  std::vector<std::ofstream> outputs;

  std::vector<std::ofstream> aOutputs;

  for (int i = 0; i < threads; ++i) {
    std::string file = "error_x_" + std::to_string(x) + "_threads_" +
                       std::to_string(threads) + "_thread_" +
                       std::to_string(i) + ".csv";
    outputs.emplace_back(file);

    // ifdef for max over all a here
    std::string aFile = "max_over_all_a_error_x_" + std::to_string(x) +
                        "_threads_" + std::to_string(threads) + "_thread_" +
                        std::to_string(i) + ".csv";
    aOutputs.emplace_back(aFile);
  }
  const auto start{std::chrono::steady_clock::now()};

  computeAllWithMultiThreading(n, x, threads, primePowers, outputs, aOutputs, whichDenom);

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