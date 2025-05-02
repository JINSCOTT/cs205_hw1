#include <iostream>
#include "al.h"
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

// Expected input format:
// hw1 <search mothod> <data file> e.g. hw1 {1, 2, 3} input.txt
// 1 for Uniform Cost Search
// 2 for A* with Manhattan distance
// 3 for A* with Misplaced Tile heuristic


int main( int argc, char* argv[] ) {
    std::ios_base::sync_with_stdio(false);

    //Input checks
    if(argc != 3) {
        std::cout << "Usage: " << argv[0] << " <size> <data file>" << std::endl;
        return 1;
    }
    int mode = std::stoi(argv[1]);
    std::string data_file = argv[2];

    //Read the data file
    std::ifstream file;
    std::string line;

    file.open(data_file);
    if(!file.is_open()) {
        std::cerr << "Error: Could not open file " << data_file << std::endl;
        return 1;
    }

    // Read the data file to vector<int>
    std::vector<int> data;
    while(std::getline(file, line)) {
        std::istringstream ss(line);
        int value;
        while(ss >> value) {
            data.push_back(value);
        }
    }
    file.close();

    //Starting the puzzle part

    // Create the problem object
    std::unique_ptr<Problem> p;
    try {
        p = std::make_unique<Problem>(data);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    int result = 0;
    switch(mode) {
        case 1:
            result = GeneralSearch(*p, QueueingFunction, SearchType::Uniform_Cost);
            break;
        case 2:
            result = GeneralSearch(*p, QueueingFunction,SearchType::AStar_Manhattan); 
            break;
        case 3:
            result = GeneralSearch(*p, QueueingFunction, SearchType::Astar_Missplaced_Tile);
            break;
        default:
            std::cerr << "Error: Invalid search method" << std::endl;
            return 1;
    }
       

 
   if(result == -1) {
        std::cout << "No solution found" << std::endl;
    } else {
        std::cout << "Solution found with cost: " << result << std::endl;
    }

    return 0;
}