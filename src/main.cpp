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
    std::cout
        << "Usage: ./main <x> <n> <threads> --commands (find in readme)\n";
    return 1;
  }
  uint64_t x = std::stoull(argv[1]);
  uint64_t n = std::stoull(argv[2]);
  int threads = std::stoi(argv[3]);

  bool primePowers = false;
  int whichDenom = 0;
  for (int i = 4; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--primePowers") {
      primePowers = true;
    }
    if (arg == "--sqrt") {
      whichDenom = 1;
    }
  }

  std::vector<std::ofstream> outputs;
  std::vector<std::ofstream> aOutputs;
  std::vector<std::ofstream> gapOutputs;
  std::vector<std::ofstream> firstOutputs;

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

    std::string gapFile = "largest_gap_in_ap_x_" + std::to_string(x) +
                          "_threads_" + std::to_string(threads) + "_thread_" +
                          std::to_string(i) + ".csv";
    gapOutputs.emplace_back(gapFile);

    std::string firstFile = "first_prime_in_ap_x_" + std::to_string(x) +
                            "_threads_" + std::to_string(threads) + "_thread_" +
                            std::to_string(i) + ".csv";
    firstOutputs.emplace_back(firstFile);
  }
  const auto start{std::chrono::steady_clock::now()};

  computeAllWithMultiThreading(n, x, threads, primePowers, outputs, aOutputs, gapOutputs, firstOutputs,
                               whichDenom);

  const auto finish{std::chrono::steady_clock::now()};
  const std::chrono::duration<double> elapsed_seconds{finish - start};

  std::ofstream timing("timing_data.txt");
  timing << "time to compute: " << elapsed_seconds.count() << std::endl;

  return 0;
}
