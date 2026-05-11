#include "PrimeCharacteristics.hpp"


uint64_t largestGap(const std::vector<uint64_t>& primes){
    if(primes.size() <= 1){
        return 0;
    }
    uint64_t gap = 0;
    for(int i = 0; i < primes.size() - 1; ++i){
        gap = std::max(gap, primes[i + 1] - primes[i]);
    } 
    return gap;
}