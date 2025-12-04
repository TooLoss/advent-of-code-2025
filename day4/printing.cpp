#include <vector>
#include <fstream>
#include <iostream>
#include <array>

class Matrix2D {

public:

    Matrix2D(std::ifstream &file) {
        std::string line;
        int i = 0;
        while (getline(file, line)) {
            matrix.push_back({});
            matrix[i].resize(line.length()-1);
            for (int j = 0 ; j < line.length() ; j++) {
                matrix[i][j] = (line[j] == '@');
            }
            i++; 
            columns = line.length();
        }
        rows = i;
    }

    bool get_pixel(int x, int y) {
        if (x >= 0 & x < rows & y >= 0 & y < columns)
            return matrix[x][y];
        else
            return false;
    }

    void set_pixel(int x, int y, bool value) {
        if (x >= 0 & x < rows & y >= 0 & y < columns)
            matrix[x][y] = value;
    }

    std::vector<bool> get_surrounding(int x, int y, int radius) {
        std::vector<bool> surrounding;
        surrounding.reserve(radius*radius);
        for (int i = -radius ; i <= radius ; i++) {
            for (int j = -radius ; j <= radius ; j++) {
                surrounding.push_back(get_pixel(x+i, y+j));
            }
        }
        return surrounding;
    }

    void print() {
        for (int i = 0 ; i < rows ; i++) {
            for (int j = 0 ; j < columns ; j++) {
                if (matrix[i][j]) {
                    std::cout << '@';
                } else {
                    std::cout << ' ';
                }
            }
            std::cout << std::endl;
        }
    }

    int columns;
    int rows;

private:
    std::vector<std::vector<bool>> matrix;
};


bool forkliftable(Matrix2D &matrix, int x, int y) {
    std::vector<bool> surrounding = matrix.get_surrounding(x, y, 1);
    int sum = 0;
    for (auto s : surrounding) {
        if (s == true)
            sum += 1;
    }
    return sum <= 4;
}

int number_forkliftable(Matrix2D &matrix) {
    int sum = 0;
    std::vector<std::array<int, 2>> to_delete;
    to_delete.reserve(matrix.rows*matrix.columns);
    for (int i = 0 ; i < matrix.rows ; i++) {
        for (int j = 0 ; j < matrix.columns ; j ++) {
            if (matrix.get_pixel(i, j) == true && forkliftable(matrix, i, j)) {
                sum += 1;
                to_delete.push_back({i, j});
            }
        }
    }
    for (auto pos : to_delete) {
        matrix.set_pixel(pos[0], pos[1], false);
    }
    return sum;
}

int shrink(Matrix2D &matrix) {
    int sum = 0;
    int new_sum = -1;
    bool exit = true;
    while (exit) {
        new_sum = sum + number_forkliftable(matrix);
        if (new_sum != sum) {
            sum = new_sum;
        } else {
            exit = false; 
        }
    }
    return new_sum;
}

int main(int argc, char* argv[]) {
    if (argc != 2) return 0; 

    std::string filename = argv[1];
    std::ifstream file{filename};
    Matrix2D matrix{file};

    int sum = 0;
    sum = shrink(matrix);
    std::cout << sum << std::endl;
}
