#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>


#define ULONGI long long int


void parse_dash(std::string pair, ULONGI &begin, ULONGI &end) {
    size_t separator_pos = pair.find('-');
    if (separator_pos != -1) {
        begin = std::stoll(pair.substr(0, separator_pos));
        end = std::stoll(pair.substr(separator_pos+1));
    } else {
        begin = 0;
        end = 0;
    } 
}


void make_database(std::ifstream &file, std::vector<std::pair<ULONGI, ULONGI>> &database) {
    std::string line;
    while (getline(file, line)) {
        if (line == "") break;
        
        ULONGI begin;
        ULONGI end;
        parse_dash(line, begin, end);
        database.push_back({begin, end});
    }
}


ULONGI card_union_database(std::vector<std::pair<ULONGI, ULONGI>> &database) {
    std::vector<std::pair<ULONGI, int>> points;
    for (const auto &interval : database) {
        points.emplace_back(interval.first, 1);
        points.emplace_back(interval.second + 1, -1);
    }

    std::sort(points.begin(), points.end());

    ULONGI total = 0;
    ULONGI last = 0;
    int active = 0;

    for (const auto &point : points) {
        if (active > 0) {
            total += point.first - last;
        }
        active += point.second;
        last = point.first;
    }

    return total;
}


int main(int argc, char* argv[]) {
    if (argc != 2) return 0;
    std::string filename = argv[1];
    std::ifstream file{filename};
    if (!file.good()) return 0;
    std::vector<std::pair<ULONGI, ULONGI>> database;
    make_database(file, database);
    ULONGI count = card_union_database(database);
    std::cout << count << std::endl;  
    return 1;
}
