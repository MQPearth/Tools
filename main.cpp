#include <iostream>
#include <random>
#include <sstream>


constexpr int CHAR_LENGTH = 69;
char CHAR_ARR[CHAR_LENGTH] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%&";


int rand_num(const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, max);
    return dis(gen);
}

std::string rand_string(const int length) {
    std::stringstream ss;
    for (int i = 0; i < length; i++) {
        ss << CHAR_ARR[rand_num(CHAR_LENGTH-1)];
    }
    return ss.str();
}


int main(const int argc, char *argv[]) {
    std::string option;
    std::string param;

    if (argc == 1) {
        option = "-n";
        param = "99";
    } else if (argc == 3) {
        option = argv[1];
        param = argv[2];
    } else {
        std::cout << "help:\nrand -n 99" << std::endl;
        return 1;
    }

    if (option == "-n") {
        std::cout << rand_num(std::stoi(param)) << std::endl;
    } else if (option == "-s") {
        std::cout << rand_string(std::stoi(param)) << std::endl;
    } else {
        std::cout << option << " is unsupport" << std::endl;
    }

    return 0;
}
