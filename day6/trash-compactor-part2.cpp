#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <cstdint>
#include <charconv>
#include <algorithm>

using ULONGI = uint64_t;

size_t parse_number(std::vector<std::string> &numbers, std::string_view line, char separator = ' ') {
    size_t start_cursor = 0;
    size_t columns = 0;
    size_t max = 0;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == separator) {
            const std::string_view sub_string = line.substr(start_cursor, i - start_cursor);
            if (!sub_string.empty()) {
                numbers.emplace_back(sub_string);
                columns++;
                if (max < sub_string.size()) max = sub_string.size();
            }
            start_cursor = i + 1;
        }
    }
    if (start_cursor < line.size()) {
        const std::string_view sub_string = line.substr(start_cursor);
        if (!sub_string.empty()) {
            numbers.emplace_back(sub_string);
            if (max < sub_string.size()) max = sub_string.size();
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

    const std::vector<std::string>& data() const {
        return m_data;
    }

    std::span<const char> operations() const {
        return m_operations;
    }

    std::vector<std::string> column(size_t j) const {
        if (j >= m_cols) throw std::out_of_range("Invalid column index");
        std::vector<std::string> col;
        col.reserve(m_rows);
        for (size_t i = 0; i < m_rows; ++i) {
            col.push_back(m_data[i * m_cols + j]);
        }
        return col;
    }

    size_t rows() const {
        return m_rows;
    }

    size_t cols() const {
        return m_cols;
    }

    void print() const {
        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < m_cols; j++) {
                std::cout << " " << m_data[j + m_cols * i] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    std::vector<char> m_operations;
    std::vector<std::string> m_data;
    size_t m_rows;
    size_t m_cols;
};

ULONGI get_number(const std::vector<std::string> &numbers, size_t j, bool is_left) {
    std::string number;
    for (const std::string &num : numbers) {
        const size_t index = is_left ? j : num.length() - j - 1;
        if (index < num.length() && index >= 0) {
            number += num[index];
        }
    }
    ULONGI value;
    auto [ptr, ec] = std::from_chars(number.data(), number.data() + number.size(), value);
    return value;
}

ULONGI make_operation(const std::vector<std::string> &numbers, char operation, bool is_left = true) {
    if (numbers.empty()) return 0;
    size_t max_len = 0;
    for (const auto &str : numbers) {
        max_len = std::max(max_len, str.length());
    }
    ULONGI sum = get_number(numbers, 0, is_left);
    for (size_t j = 1; j < max_len; j++) {
        ULONGI num = get_number(numbers, j, is_left);
        switch (operation) {
            case '*':
                sum *= num;
                break;
            case '+':
                sum += num;
                break;
        }
    }
    return sum;
}

ULONGI table_sum(Database &database) {
    const std::span<const char> operations = database.operations();
    ULONGI sum = 0;
    for (size_t i = 0; i < database.cols(); i++) {
        const std::vector<std::string> numbers = database.column(i);
        if (i < operations.size()) {
            bool is_left = numbers[0].length() > numbers[numbers.size() - 1].length();
            sum += make_operation(numbers, operations[i], is_left);
        }
    }
    return sum;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file{filename};
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return 1;
    }
    Database database{file};
    ULONGI sum = table_sum(database);
    std::cout << sum << std::endl;
}

