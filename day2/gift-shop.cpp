#include <string_view>
#include <iostream>
#include <fstream>
#include <sstream>

bool is_invalid(std::string_view number, int time = 2) {
    if (number.length() % time != 0) return false;

    long long int center = number.length() / time;

    bool repeated = true;
    for (size_t i = 0 ; i < center ; i++) {
        for (size_t j = 0 ; j < time ; j++) {
            repeated = repeated * number[i + j * center] == number[i];
        }
    }

    return repeated;
}

bool multiple_invalid(std::string_view number) {
    bool repeated = true;
    for (size_t i = 0 ; i < number.length() ; i++) {
        
    }
}

unsigned long long int add_invalid_in_range(const unsigned long long int &begin, const unsigned long long int &end) {
    unsigned long long int sum = 0;
    for (size_t i = begin ; i <= end ; i++) {
        if (is_invalid(std::to_string(i))) {
            sum += i;
        }
    }
    return sum;
}

void parse_dashed(std::string pair, unsigned long long int &begin, unsigned long long int &end) {
    int separator_pos = pair.find('-');
    if (separator_pos != -1) {
        begin = std::stoul(pair.substr(0, separator_pos));
        end = std::stoul(pair.substr(separator_pos+1));
    } else {
        begin = 0;
        end = 0;
    }
}

void parse_line(std::string line, unsigned long long int &sum) {
    std::istringstream iss(line);
    std::string pair;

    unsigned long long int begin;
    unsigned long long int end;

    while (std::getline(iss, pair, ',')) {
        parse_dashed(pair, begin, end);
        std::cout << begin << std::endl;
        std::cout << end << std::endl;
        sum += add_invalid_in_range(begin, end);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Require 1 parameter" << std::endl;
        return 0;
    }

    std::string_view filename = argv[1];
    std::ifstream file{filename};

    if (!file.good()) return 0;

    unsigned long long int sum = 0;
    std::string line;
    while (std::getline(file, line)) {
        parse_line(line, sum);
    }

    std::cout << sum << std::endl;
}
