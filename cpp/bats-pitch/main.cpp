#include "vol_calc.h"

namespace pb = pitch::bats;

constexpr int NUM_TOP_PRODUCTS = 10;

int main() {
  pb::TopVolumeCalculator<NUM_TOP_PRODUCTS> calculator;
  calculator.process(std::cin);
  std::cout << calculator;
}
