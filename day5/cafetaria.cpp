#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>


#define ULONGI unsigned long int


void parse_dash(std::string pair, ULONGI &begin, ULONGI &end) {
    int separator_pos = pair.find('-');
    if (separator_pos != -1) {
        begin = std::stoul(pair.substr(0, separator_pos));
        end = std::stoul(pair.substr(separator_pos+1));
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


bool is_fresh(const ULONGI id, const std::vector<std::pair<ULONGI, ULONGI>> &database) {
    for (std::pair<ULONGI, ULONGI> pair : database) {
        if (id >= pair.first && id <= pair.second) return true;
    }

    return false;
}


// part 1
int fresh_count(std::ifstream &file, const std::vector<std::pair<ULONGI, ULONGI>> &database) {
    int count = 0;
    std::string line;
    while (getline(file, line)) {
        const ULONGI id = std::stoul(line);
        if (is_fresh(id, database)) count++;
    }
    return count;
}


int main(int argc, char* argv[]) {
    if (argc != 2) return 0;

    std::string filename = argv[1];
    std::ifstream file{filename};

    if (!file.good()) return 0;

    std::vector<std::pair<ULONGI, ULONGI>> database;
    make_database(file, database);
    int count = fresh_count(file, database);

    std::cout << count << std::endl;  

    return 1;
}
