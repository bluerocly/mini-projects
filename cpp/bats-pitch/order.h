//
// Created by ksharma on 8/13/18.
//

#ifndef BATS_PITCH_ORDER_H
#define BATS_PITCH_ORDER_H

#include <string>
struct Order {
  std::string order_id;
  char side;
  uint32_t quantity;
  std::string symbol;
  uint32_t price;
};

#endif //BATS_PITCH_ORDER_H
