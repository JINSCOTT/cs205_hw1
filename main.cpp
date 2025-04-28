#include <iostream>
#include "al.h"
#include <vector>
#include <memory>
// Expected input format:
// hw1 <size> <data file> e.g. hw1 3 input.txt


int main( int argc, char* argv[] ) {
    std::ios_base::sync_with_stdio(false)

    //Input checks
    if(argc != 3) {
        std::cout << "Usage: " << argv[0] << " <size> <data file>" << std::endl;
        return 1;
    }
    int size = std::stoi(argv[1]);
    std::string data_file = argv[2];

    // Create a puzzle object
    std::unique_ptr<puzzle> p;
    try {
        p = std::make_unique<puzzle>(size, data_file);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    // Print the puzzle
    std::cout << "Initial state Puzzle:\n";
    p->print();
   
}