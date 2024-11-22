#include <iostream>
#include <random>
#include <thread>
#include <chrono>


constexpr int CHAR_LENGTH = 69;
char CHAR_ARR[CHAR_LENGTH] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%&";


int rand(const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, max);
    return dis(gen);
}

void rand_num(const int max) {
    for (int i = 0; i < 30000; ++i) {
        std::cout << rand(max) << std::flush;
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        std::cout << "\r";
    }
    std::cout << rand(max) << std::endl << std::flush;
}

void rand_string(const long long length) {
    for (int i = 0; i < length; i++) {
        std::cout << CHAR_ARR[rand(CHAR_LENGTH - 1)] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << std::endl << std::flush;
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
        rand_num(std::stoi(param));
    } else if (option == "-s") {
        rand_string(std::stoll(param));
    } else {
        std::cout << option << " is unsupport" << std::endl;
    }

    return 0;
}
