#pragma once
#include <gmp.h>
#include <primesieve.hpp>

#include <vector>
#include <iostream>
#include <algorithm>

/*
finds the largest gap between two primes, given a list of primes.
If the list has <= 1 elements then it returns 0.
This is purely to get a feel for things like primesieve and time benchmarks.
*/
uint64_t largestGap(const std::vector<uint64_t>& primes);


