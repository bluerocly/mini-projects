#include "vol_calc.h"

namespace pb = pitch::bats;

int main() {
  freopen("./pitch_example_data", "r", stdin);
  pb::TopVolumeCalculator<10> calculator;
  calculator.process(std::cin);
  std::cout << calculator;
}
