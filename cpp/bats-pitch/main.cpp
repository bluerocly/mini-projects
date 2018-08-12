#include <iostream>
#include <string>
#include <iomanip>

enum class MsgType {
    ADD_ORDER_SHORT = 'A',
    ADD_ORDER_LONG = 'd',
    ORDER_EXECUTED = 'E',
    ORDER_CANCEL = 'X',
    TRADE_SHORT = 'P',
    TRADE_LONG = 'r'
};

int parse_int(std::istream& in, int width) {
    std::string value_str;
    in >> std::setw(width) >> value_str;
    return std::stoi(value_str);
}

double parse_double(std::istream& in, int width) {
    std::string value_str;
    in >> std::setw(width) >> value_str;
    return std::stod(value_str);
}

int main() {
//    freopen("./pitch_example_data", "r", stdin);

    int64_t timestamp;
    char message_type;

    uint32_t count = 1;

    while (std::cin.good()) {
        std::cout << "Processing " << count << " -> ";

        std::cin.ignore(1);
        std::cin >> std::setw(8) >> timestamp;
        std::cin >> message_type;

        if (MsgType(message_type) == MsgType::ADD_ORDER_SHORT) {

            std::cout << "ADDED S : ";

            std::string order_id(12, ' ');
            std::cin.read(&order_id[0], 12);

            char side;
            std::cin.get(side);

            uint32_t shares;
            std::cin >> std::setw(6) >> shares;

            std::string symbol(6, ' ');
            std::cin.read(&symbol[0], 6);

            double price;
            std::cin >> std::setw(10) >> price;

            price *= 0.0001;

            char displayed;
            std::cin >> displayed;

            char end_line;
            std::cin.get(end_line);

            std::cout << timestamp << " | " << message_type << " | " << order_id << " | " << side << " | " << shares << " | " << symbol << " | " << price << std::endl;

        } else if (MsgType(message_type) == MsgType::ADD_ORDER_LONG) {

            std::cout << "ADDED L :";

            std::string order_id(12, ' ');
            std::cin.read(&order_id[0], 12);

            char side;
            std::cin.get(side);

            uint32_t shares;
            std::cin >> std::setw(6) >> shares;

            std::string symbol(8, ' ');
            std::cin.read(&symbol[0], 8);

            double price;
            std::cin >> std::setw(10) >> price;

            price *= 0.0001;

            char displayed;
            std::cin >> displayed;

            char end_line;
            std::cin.get(end_line);

            std::cout << timestamp << " | " << message_type << " | " << order_id << " | " << side << " | " << shares << " | " << symbol << " | " << price << std::endl;

        }
        else if (MsgType(message_type) == MsgType::ORDER_EXECUTED) {

            std::cout << "EXEC : ";

            std::string order_id(12, ' ');
            std::cin.read(&order_id[0], 12);

            int32_t shares = parse_int(std::cin, 6);

            std::string exec_id(12, ' ');
            std::cin.read(&exec_id[0], 12);

            char end_line;
            std::cin >> end_line;

            std::cout << timestamp << " | " << message_type << " | " << order_id << " | " << shares << " | " << exec_id << std::endl;

        } else if (MsgType(message_type) == MsgType::ORDER_CANCEL) {

            std::cout << "CANCEL : ";

            std::string order_id;
            std::cin >> std::setw(12) >> order_id;

            int32_t shares = parse_int(std::cin, 6);

            char end_line;
            std::cin >> end_line;

            std::cout << timestamp << " | " << message_type << " | " << order_id << " | " << shares << std::endl;

        } else if (MsgType(message_type) == MsgType::TRADE_SHORT) {

            std::cout << "TRADE S : ";

            std::string order_id;
            std::cin >> std::setw(12) >> order_id;

            char side;
            std::cin.get(side);

            int32_t shares = parse_int(std::cin, 6);

            std::string symbol;
            std::cin >> std::setw(6) >> symbol;

            double price = parse_double(std::cin, 10) * 0.0001;

            std::string exec_id;
            std::cin >> std::setw(12) >> exec_id;

            char end_line;
            std::cin >> end_line;

            std::cout << timestamp << " | " << message_type << " | " << order_id << " | " << side << " | " << shares << " | " << symbol << " | " << price << " | " << exec_id  << std::endl;

        } else if (MsgType(message_type) == MsgType::TRADE_LONG) {

            std::cout << "TRADE L : ";

            std::string order_id;
            std::cin >> std::setw(12) >> order_id;

            char side;
            std::cin.get(side);

            int32_t shares = parse_int(std::cin, 6);

            std::string symbol;
            std::cin >> std::setw(8) >> symbol;

            double price = parse_double(std::cin, 10) * 0.0001;

            std::string exec_id;
            std::cin >> std::setw(12) >> exec_id;

            char end_line;
            std::cin >> end_line;

            std::cout << timestamp << " | " << message_type << " | " << order_id << " | " << side << " | " << shares << " | " << symbol << " | " << price << " | " << exec_id  << std::endl;

        }
        else {
//            std::cout << std::setw(10) << "ADDED";
            std::string remaining;
            std::getline(std::cin, remaining);
            std::cout << std::endl;
        }
        count ++;
    }

}
