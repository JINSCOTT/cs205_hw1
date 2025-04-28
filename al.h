#ifndef AL_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>


std::vector<std::pair<int, int>> dirs = {{0,-1}, {0,1}, {-1,0}, {1,0}};

class BaseSearch {
    public:
        virtual void search() = 0;
        virtual void print() = 0;
        virtual ~BaseSearch() = default;
};

class 



class Node {
    public:
        Node(int size, const std::vector<int>& data, int gen = 0);
        Node(const Node& other, std::pair<int, int> move, int gen);
        static Node createNode(int size, const std::string& filename);
    

        const std::vector<int>& get_state() const;
        bool operator==(const Node& other) const;
        void print() const;
        bool is_goal() const;
    
        
        int get_cost() const;
        int get_dim() const;
        int get_size() const;
    
      
    
    private:
        int size = 8;
        int dim = 3;
        int g = 0;  // cost so far
        std::vector<int> STATE;
    };


#endif // AL_H