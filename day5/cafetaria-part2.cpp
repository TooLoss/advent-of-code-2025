#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>


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


ULONGI card(std::pair<ULONGI, ULONGI> A) {
    return A.second - A.first + 1;
}


std::pair<ULONGI, ULONGI> intersection(std::pair<ULONGI, ULONGI> A, std::pair<ULONGI, ULONGI> B) {
    std::pair<ULONGI, ULONGI> inter{std::max(A.first, B.first), std::min(A.second, B.second)};
    if (inter.second < inter.first) return {-1, -1};
    else return inter;
}


std::pair<ULONGI, ULONGI> intersection_range(const std::vector<std::pair<ULONGI, ULONGI>> &database, size_t begin, size_t end) {
    std::pair<ULONGI, ULONGI> inter = database[begin];
    if (begin > end || begin < 0 || end >= database.size()) {
        return {-1, -1};
    }
    for (size_t i = begin + 1; i <= end; i++) {
        inter = intersection(inter, database[i]);
    }
    return inter;
}


std::vector<std::vector<size_t>> get_all_subset(size_t n) {
    std::vector<std::vector<size_t>> subsets;
    for (size_t mask = 1; mask < (1 << n); mask++) {
        std::vector<size_t> subset;
        for (size_t i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                subset.push_back(i);
            }
        }
        subsets.push_back(subset);
    }
    return subsets;
}


ULONGI card_union_database(std::vector<std::pair<ULONGI, ULONGI>> &database) {
    ULONGI card_total = 0;
    auto all_subset = get_all_subset(database.size());
    for (const auto &subset : all_subset) {
        std::vector<std::pair<ULONGI, ULONGI>> subset_intervals;
        for (size_t index : subset) {
            subset_intervals.push_back(database[index]);
        }
        auto inter_current = intersection_range(subset_intervals, 0, subset_intervals.size() - 1);
        ULONGI card_current = 0;
        if (inter_current.first != static_cast<ULONGI>(-1) && inter_current.second != static_cast<ULONGI>(-1)) {
            card_current = inter_current.second - inter_current.first + 1; // Include endpoints
        }
        int sign = (subset.size() % 2 == 1) ? 1 : -1;
        card_total += sign * card_current;
    }
    return card_total;
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
