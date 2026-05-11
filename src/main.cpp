#include <gmp.h>
#include <primesieve.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>

#include "PrimeCharacteristics.hpp"

int main(){
    std::vector<uint64_t> v;
    const auto start{std::chrono::steady_clock::now()};
    primesieve::generate_primes(100, &v);
    const auto finish{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{finish - start};
    std::cout << "time to generate primes: " << elapsed_seconds.count() << std::endl;
    std::cout << "number of primes: " << v.size() << std::endl;

    // for(const auto& x : v){
    //     std::cout << x << std::endl;
    // }

    const auto start1{std::chrono::steady_clock::now()};
    const auto gap{largestGap(v)};
    const auto finish1{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds1{finish1 - start1};
    std::cout << "largest gap between primes: " <<gap << std::endl << "time: " << elapsed_seconds1.count() << std::endl;

    return 0;
}
