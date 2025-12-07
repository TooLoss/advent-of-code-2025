#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>
#include <span>
#include <cstdint>
#include <charconv>


using ULONGI = uint64_t;


size_t parse_number(std::vector<ULONGI> &numbers, std::string_view line, char separator = ' ') {
    size_t start_cursor = 0;
    size_t columns = 0;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == separator) {
            const std::string_view sub_string = line.substr(start_cursor, i - start_cursor);
            if (!sub_string.empty()) {
                ULONGI num = 0;
                auto [ptr, ec] = std::from_chars(sub_string.data(), sub_string.data() + sub_string.size(), num);
                if (ec == std::errc())
                    numbers.push_back(num);
                columns++;
            }
            start_cursor = i + 1;
        }
    }
    if (start_cursor < line.size()) {
        const std::string_view sub_string = line.substr(start_cursor);
        if (!sub_string.empty()) {
            ULONGI num = 0;
            auto [ptr, ec] = std::from_chars(sub_string.data(), sub_string.data() + sub_string.size(), num);
            if (ec == std::errc())
                numbers.push_back(num);
            columns++;
        }
    }
    return columns;
}


void parse_operations(std::vector<char> &operations, std::string_view line, char separator = ' ') {
    for (char c : line) {
        if (!std::isspace(c)) {
            operations.push_back(c);
        }
    }
}


bool is_number(const std::string &line) {
    if (line.empty()) return false;
    for (char c : line) {
        if (!std::isdigit(c) && !std::isspace(c))
            return false;
    }
    return true;
}


class Database {
public:
    Database(std::ifstream &file) {
        std::string line;
        bool first_numeric_line = true;
        while (std::getline(file, line)) {
            if (is_number(line)) {
                size_t cols = parse_number(m_data, line);
                if (first_numeric_line) {
                    m_cols = cols;
                    first_numeric_line = false;
                }
            } else {
                m_operations.reserve(m_cols);
                parse_operations(m_operations, line);
            }
        }
        m_rows = m_data.size() / m_cols;
        std::cout << "Rows " << m_rows << std::endl;
        std::cout << "Columns " << m_cols << std::endl;
    }

    std::span<ULONGI> data() {
        return m_data;
    }

    std::span<char> operations() {
        return m_operations;
    }

    std::vector<ULONGI> column(size_t j) {
        if (j >= m_cols) throw std::out_of_range("Invalid column index");
        std::vector<ULONGI> col;
        col.reserve(m_rows);
        for (size_t i = 0; i < m_rows; ++i) {
            col.push_back(m_data[i * m_cols + j]);
        }
        return col;
    }

    size_t rows() {
        return m_rows;
    }

    size_t cols() {
        return m_cols;
    }

    void print() {
        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < m_cols; j++) {
                std::cout << " " << m_data[j + m_cols * i] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<ULONGI> m_data;
    std::vector<char> m_operations;

    size_t m_rows;
    size_t m_cols;
};


ULONGI make_operation(std::vector<ULONGI> numbers, char operation) {
    if (numbers.empty()) return 0;
    ULONGI sum = numbers.data()[0];
    for (size_t j = 1; j < numbers.size(); j++) {
        switch (operation) {
            case '*':
                sum *= numbers[j];
                break;
            case '+':
                sum += numbers[j];
                break;
        } 
    }
    return sum;
}


ULONGI table_sum(Database &database) {
    const std::span<char> operations = database.operations();
    ULONGI sum = 0;
    for (size_t i = 0; i < database.cols(); i++) {
        const std::vector<ULONGI> numbers = database.column(i);
        if (i < operations.size()) {
            sum += make_operation(numbers, operations[i]);
        }
    }
    return sum;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string_view filename = argv[1];
    std::ifstream file{std::string(filename)};

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return 1;   
    }

    Database database{file};
    ULONGI sum = table_sum(database);
    std::cout << sum << std::endl;

}
