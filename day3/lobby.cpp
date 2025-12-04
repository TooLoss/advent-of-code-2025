#include <iostream>
#include <fstream>
#include <string_view>

int get_maximum_index(std::string_view input) {
    int max_index = 0;
    char max = input[max_index];
    for (int i = 1 ; i < input.length() ; i++) {
        if (input[i] > max) {
            max = input[i];
            max_index = i;
        }
    }
    return max_index; 
}

int maximum_joltage(std::string input) {
    if (input == "") return 0;
    std::string input1 = input.substr(0, input.length()-1);
    int decimale = get_maximum_index(input1);
    std::string input2 = input.substr(decimale+1);
    int unite = get_maximum_index(input2);
    std::string number;
    number += input[decimale];
    number += input2[unite];
    return std::stoi(number);
}

unsigned long int maximum_njoltage(std::string input, int n) {
    std::string number;
    int pos = 0;
    int pos_sum = 0;
    int discovery = input.length() - (n-1);
    for (int i = 0 ; i < n ; i ++) {
        std::string new_input = input.substr(pos_sum, discovery - pos_sum);
        // std::cout << "input : " << new_input << std::endl;
        int pos = get_maximum_index(new_input);
        number += new_input[pos];
        pos_sum += pos + 1;
        discovery += 1;
        // std::cout << "disovery : " << discovery << std::endl;
        // std::cout << "pos : " << pos << std::endl;
        // std::cout << "sum pos : " << pos_sum << std::endl;
    }
    return std::stoul(number);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Wrong parameters" << std::endl;
        return 0;
    }

    std::string_view filepath = argv[1];
    std::ifstream file{filepath};

    if (!file.good()) return 0;

    std::string line;
    unsigned long sum = 0;
    while (std::getline(file, line)) {
        sum += maximum_njoltage(line, 12); 
    }

    std::cout << sum << std::endl;
}
