#pragma once
#include <gmp.h>

#include <algorithm>
#include <iostream>
#include <primesieve.hpp>
#include <unordered_map>
#include <vector>

/*
finds the largest gap between two primes, given a list of primes.
If the list has <= 1 elements then it returns 0.
This is purely to get a feel for things like primesieve and time benchmarks.
*/

uint64_t largestGap(const std::vector<uint64_t>& primes);

/*
Computes the number of primes in each arithmetic progression
Inputs: x - the upper limit of primes we look for.
        n - the modulus with which we use to find a.
For each prime (p) < x, this function updates the characteristics of a (p mod
n), such as incrementing count[a].
time complexity: O(pi(x)), as the operations we perform in the while loop take
constant time. returns a vector of size n, which is the number of arithmetic
progressions we use.
*/

std::vector<uint64_t> primesModN(const uint64_t n, const uint64_t x);

/*
computes the primes mod n for all n less than upperBound, with x as the upper
limit of primes we look for.
*/

void computeAll(const uint64_t upperBound, const uint64_t x);

/*
adds up all the values in a vector. Used to verify that the number of primes
adds up when testing function primesModN.
*/

int add(const std::vector<uint64_t>& v);
