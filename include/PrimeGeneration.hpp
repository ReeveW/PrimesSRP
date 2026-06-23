#pragma once

#include <primesieve.hpp>
#include <vector>
#include <queue>

struct primePowerElement{
    uint64_t value, power;
};

extern std::vector<uint64_t> primeList;

extern std::vector<primePowerElement> primePowerList;

void fillPrimeList(uint64_t x);

void fillPrimeListWithPowers(uint64_t x);
