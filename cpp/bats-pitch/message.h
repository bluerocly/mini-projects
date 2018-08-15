#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>

#ifndef BATS_PITCH_PITCHMESSAGE_H
#define BATS_PITCH_PITCHMESSAGE_H

namespace pitch {
namespace bats {

typedef std::string Alpha;
typedef std::string Base36Numeric;
typedef uint32_t Numeric;
typedef uint32_t Prices; // We can keep the prices as ints since we are not doing any calculation
typedef uint32_t Timestamp;

template<typename T>
T parse(std::istream &in, int width);

template<>
std::string parse<std::string>(std::istream &in, int width) {
  std::string rv;
  in >> std::setw(width) >> rv;
  return rv;
}

template<>
uint32_t parse<uint32_t>(std::istream &in, int width) {
  std::string rv_str;
  in >> std::setw(width) >> rv_str;
  return static_cast<uint32_t>(std::stoi(rv_str));
}

template<>
double_t parse<double_t>(std::istream &in, int width) {
  std::string rv_str;
  in >> std::setw(width) >> rv_str;
  return std::stod(rv_str);
}

template<>
char parse<char>(std::istream &in, int width) {
  // Width is ignore since width = 1
  char rv;
  in >> rv;
  return rv;
}

enum class MessageType : char {
  ADD_ORDER_SHORT = 'A',
  ADD_ORDER_LONG = 'd',
  ORDER_EXECUTED = 'E',
  ORDER_CANCEL = 'X',
  TRADE_SHORT = 'P',
  TRADE_LONG = 'r'
};

struct PitchMessageheader {
  static constexpr int TIMESTAMP_SIZE = 8;

  uint32_t timestamp;
  MessageType message_type;

  PitchMessageheader(std::istream &in) : timestamp(parse<Timestamp>(in, TIMESTAMP_SIZE)),
                                         message_type(MessageType(parse<char>(in, 1))) {}

};

template<typename T>
class PitchMessage {

 private:
  PitchMessageheader header_;
 public:

  PitchMessage(const PitchMessageheader &header) : header_(header) {}

  const PitchMessageheader &header() const {
    return header_;
  }
};

class AddOrderShortMessage : public PitchMessage<AddOrderShortMessage> {

  static constexpr int ORDER_ID_SIZE = 12;
  static constexpr int SHARES_SIZE = 6;
  static constexpr int SYMBOL_SIZE = 6;
  static constexpr int PRICE_SIZE = 10;

 private:
  std::string order_id_;
  char side_;
  uint32_t shares_;
  std::string symbol_;
  uint32_t price_;
  char displayed_;

 public:
  AddOrderShortMessage(const PitchMessageheader &header, std::istream &in) :
      PitchMessage<AddOrderShortMessage>(header), order_id_(parse<Base36Numeric>(in, ORDER_ID_SIZE)),
      side_(parse<char>(in, 1)), shares_(parse<Numeric>(in, SHARES_SIZE)), symbol_(parse<Alpha>(in, SYMBOL_SIZE)),
      price_(parse<Numeric>(in, PRICE_SIZE)), displayed_(parse<char>(in, 1)) {
  }

  const std::string &order_id() const {
    return order_id_;
  }

  char side() const {
    return side_;
  }

  uint32_t shares() const {
    return shares_;
  }

  const std::string &symbol() const {
    return symbol_;
  }

  uint32_t price() const {
    return price_;
  }

};

class AddOrderLongMessage : public PitchMessage<AddOrderLongMessage> {

  static constexpr int ORDER_ID_SIZE = 12;
  static constexpr int SHARES_SIZE = 6;
  static constexpr int SYMBOL_SIZE = 8;
  static constexpr int PRICE_SIZE = 10;

 private:
  std::string order_id_;
  char side_;
  uint32_t shares_;
  std::string symbol_;
  uint32_t price_;
  char displayed_;

 public:
  AddOrderLongMessage(const PitchMessageheader &header, std::istream &in) :
      PitchMessage<AddOrderLongMessage>(header), order_id_(parse<Base36Numeric>(in, ORDER_ID_SIZE)),
      side_(parse<char>(in, 1)), shares_(parse<Numeric>(in, SHARES_SIZE)), symbol_(parse<Alpha>(in, SYMBOL_SIZE)),
      price_(parse<Numeric>(in, PRICE_SIZE)), displayed_(parse<char>(in, 1)) {
  }

  const std::string &order_id() const {
    return order_id_;
  }

  char side() const {
    return side_;
  }

  uint32_t shares() const {
    return shares_;
  }

  const std::string &symbol() const {
    return symbol_;
  }

  uint32_t price() const {
    return price_;
  }

};

class OrderExecutionMessage : public PitchMessage<OrderExecutionMessage> {

  static constexpr int ORDER_ID_SIZE = 12;
  static constexpr int SHARES_SIZE = 6;
  static constexpr int EXEC_ID_SIZE = 12;

 private:
  std::string order_id_;
  uint32_t shares_;
  std::string exec_id_;

 public:
  OrderExecutionMessage(const PitchMessageheader &header, std::istream &in) :
      PitchMessage<OrderExecutionMessage>(header), order_id_(parse<Base36Numeric>(in, ORDER_ID_SIZE)),
      shares_(parse<Numeric>(in, SHARES_SIZE)),
      exec_id_(parse<Base36Numeric>(in, EXEC_ID_SIZE)) {
  }

  const std::string &order_id() const {
    return order_id_;
  }

  uint32_t shares() const {
    return shares_;
  }

  const std::string &exec_id() const {
    return exec_id_;
  }
};

class OrderCancelMessage : public PitchMessage<OrderCancelMessage> {

  static constexpr int ORDER_ID_SIZE = 12;
  static constexpr int SHARES_SIZE = 6;

 private:
  std::string order_id_;
  uint32_t shares_;

 public:
  OrderCancelMessage(const PitchMessageheader &header, std::istream &in) :
      PitchMessage<OrderCancelMessage>(header), order_id_(parse<Base36Numeric>(in, ORDER_ID_SIZE)),
      shares_(parse<Numeric>(in, SHARES_SIZE)) {
  }

  const std::string &order_id() const {
    return order_id_;
  }

  uint32_t shares() const {
    return shares_;
  }
};

class TradeShortMessage : public PitchMessage<TradeShortMessage> {

  static constexpr int ORDER_ID_SIZE = 12;
  static constexpr int SHARES_SIZE = 6;
  static constexpr int SYMBOL_SIZE = 6;
  static constexpr int PRICE_SIZE = 10;
  static constexpr int EXEC_ID_SIZE = 12;

 private:
  std::string order_id_;
  char side_;
  uint32_t shares_;
  std::string symbol_;
  uint32_t price_;
  std::string exec_id_;
 public:
  TradeShortMessage(const PitchMessageheader &header, std::istream &in) :
      PitchMessage<TradeShortMessage>(header), order_id_(parse<Base36Numeric>(in, ORDER_ID_SIZE)),
      side_(parse<char>(in, 1)), shares_(parse<Numeric>(in, SHARES_SIZE)), symbol_(parse<Alpha>(in, SYMBOL_SIZE)),
      price_(parse<Numeric>(in, PRICE_SIZE)), exec_id_(parse<Base36Numeric>(in, EXEC_ID_SIZE)) {
  }

  const std::string &order_id() const {
    return order_id_;
  }

  char side() const {
    return side_;
  }

  uint32_t shares() const {
    return shares_;
  }

  const std::string &symbol() const {
    return symbol_;
  }

  double_t price() const {
    return price_;
  }

  const std::string &exec_id() const {
    return exec_id_;
  }
};

class TradeLongMessage : public PitchMessage<TradeLongMessage> {

  static constexpr int ORDER_ID_SIZE = 12;
  static constexpr int SHARES_SIZE = 6;
  static constexpr int SYMBOL_SIZE = 8;
  static constexpr int PRICE_SIZE = 10;
  static constexpr int EXEC_ID_SIZE = 12;

 private:
  std::string order_id_;
  char side_;
  uint32_t shares_;
  std::string symbol_;
  uint32_t price_;
  std::string exec_id_;
 public:
  TradeLongMessage(const PitchMessageheader &header, std::istream &in) :
      PitchMessage<TradeLongMessage>(header), order_id_(parse<Base36Numeric>(in, ORDER_ID_SIZE)),
      side_(parse<char>(in, 1)), shares_(parse<Numeric>(in, SHARES_SIZE)), symbol_(parse<Alpha>(in, SYMBOL_SIZE)),
      price_(parse<Numeric>(in, PRICE_SIZE)), exec_id_(parse<Base36Numeric>(in, EXEC_ID_SIZE)) {
  }

  const std::string &order_id() const {
    return order_id_;
  }

  char side() const {
    return side_;
  }

  uint32_t shares() const {
    return shares_;
  }

  const std::string &symbol() const {
    return symbol_;
  }

  double_t price() const {
    return price_;
  }

  const std::string &exec_id() const {
    return exec_id_;
  }
};

}
}

#endif //BATS_PITCH_PITCHMESSAGE_H
