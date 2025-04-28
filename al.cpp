#include"al.h"
#include"cmath"

puzzle::puzzle(int size, std::string data_file) {

    this->puzzle_size = size;
    std::ifstream inputfile(data_file);
    std::vector<int> seen(this->puzzle_size + 1, 0);
    this->sq = std::sqrt(this->puzzle_size + 1);
    std::string line;
    // read the data from the file
    //ref: https://www.geeksforgeeks.org/stringstream-c-applications/
    while (getline(inputfile, line)) {
        std::stringstream ss(line);
        int num;
        while (ss >> num) {
            if (num < 0 || num > this->puzzle_size+1 || seen[num]>0) {
                std::cerr << "Invalid puzzle input. Content.\n";
                throw std::invalid_argument("Invalid puzzle input. Content");
            }
            this->data.push_back(num);
            seen[num]++;
        }
    }

    // Check if the data contains valid values
    if ((int)this->data.size() != this->puzzle_size +1) {
        std::cerr << "Invalid puzzle input. Size.\n";
        throw std::invalid_argument("Invalid puzzle input. Size.");
    }
  
}

bool puzzle::is_solved() {
    for (int i = 0; i < this->puzzle_size+1; i++) {
        if (this->data[i] != i + 1) {
            return false;
        }
    }
    return true;
}

void puzzle::print() {
    for (int i = 0; i < this->sq; i++) {
        for (int j = 0; j < this->sq; j++) {
            std::cout << this->data[i * this->sq + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
