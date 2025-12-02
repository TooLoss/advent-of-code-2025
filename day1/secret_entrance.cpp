#include <fstream>
#include <string_view>
#include <iostream>

int read_rotation(std::string line_text) {
    int multiplier = 1;
    if (line_text[0] == 'L') multiplier = -1;

    const int value = std::stoi(line_text.substr(1));

    std::cout << "value : " << value << std::endl;
    std::cout << "multiplier : " << multiplier << std::endl;

    return multiplier * value;
};

void count_pass_zero(std::ifstream &file) {

    int rotation = 50;
    int base_rotation = 50;
    int zero_count = 0;

    if (!file.good()) return;

    std::string line_text;

    while (getline(file, line_text)) {
        if (!line_text.empty()) {

            std::cout << "base rotation : " << rotation << std::endl;
            base_rotation = rotation;

            rotation += read_rotation(line_text);


            if (rotation > 99) {
                zero_count += rotation / 100;
                rotation = rotation % 100;
            } else if (rotation <= 0) {
                if (base_rotation % 100 == 0) {
                    zero_count += (-rotation)/100;
                } else {
                    zero_count += (-rotation+100)/100;
                }
                rotation = (100 + rotation % 100)%100;
            }

            std::cout << "rotation out : " << rotation << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;

            base_rotation = rotation;
        }
    }

    std::cout << zero_count << std::endl;
};

int main(int argc, char *argv[]) {
    if (argc != 2)
        std::cout << "Parameter filename missing" << std::endl;

    std::string_view filename = argv[1];
    std::ifstream file{filename};

    count_pass_zero(file);

};
