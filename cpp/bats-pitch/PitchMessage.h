#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>

#ifndef BATS_PITCH_PITCHMESSAGE_H
#define BATS_PITCH_PITCHMESSAGE_H

namespace pitch {
    namespace bats {
        constexpr size_t TIMESTAMP_SIZE = 8;


        typedef std::string Alpha;
        typedef std::string Base36Numeric;
        typedef uint32_t Numeric;
        typedef double_t Prices;
        typedef uint32_t Timestamp;

        template <typename T>
        T parse(std::istream& in, int width);

        template <>
        std::string parse<std::string>(std::istream& in, int width) {
            std::string rv;
            in >> std::setw(width) >> rv;
            return rv;
        }

        template <>
        uint32_t parse<uint32_t>(std::istream& in, int width) {
            std::string rv_str;
            in >> std::setw(width) >> rv_str;
            return static_cast<uint32_t>(std::stoi(rv_str));
        }

        template <>
        double_t parse<double_t >(std::istream& in, int width) {
            std::string rv_str;
            in >> std::setw(width) >> rv_str;
            return std::stod(rv_str);
        }

        enum class MessageType : char {
            ADD_ORDER_SHORT = 'A',
            ADD_ORDER_LONG  = 'd',
            ORDER_EXECUTED  = 'E',
            ORDER_CANCEL    = 'X',
            TRADE_SHORT     = 'P',
            TRADE_LONG      = 'r'
        };

        template <typename T>
        class PitchMessage {
        private:
            uint32_t timestamp_;
            MessageType message_type_;
        public:
            void parse_from_stream(std::istream& in) {
                timestamp_ = parse<Timestamp>(in, TIMESTAMP_SIZE);
            }

            inline Timestamp timestamp() const {
                return timestamp_;
            }
        };

        class AddOrderShortMessage : public PitchMessage<AddOrderShortMessage> {
            void parse_from_message(std::istream in) {
                PitchMessage<AddOrderShortMessage>::parse_from_stream(in);
            }
        };
    }
}


#endif //BATS_PITCH_PITCHMESSAGE_H
