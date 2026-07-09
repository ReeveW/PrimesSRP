#include "Denominators.hpp"

ThetaErrorTermDenominators denominator;

void ThetaErrorTermDenominators::setDenominator(int whichDenominator) {
  switch (whichDenominator) {
    case 0:
      denominatorFunction = tripleLogDenom;
      break;
    case 1:
      denominatorFunction = sqrtDenom;
      break;
    default:
      throw std::runtime_error("denominator you tried to select doesn't exist");
  }
  return;
}

long double ThetaErrorTermDenominators::tripleLogDenom(uint64_t x) {
  return std::sqrt(static_cast<long double>(x)) *
         std::pow(std::log(std::log(std::log(static_cast<long double>(x)))), 2);
}

long double ThetaErrorTermDenominators::sqrtDenom(uint64_t x) {
    return std::sqrt(static_cast<long double>(x));
}


