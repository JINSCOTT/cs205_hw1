#ifndef AL_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <queue>



int general_search( problem &p, )

void ucs(std::priority_queue<Node, std::vector<Node>, NodeComparator> &open,
         std::unordered_set<std::string> &closed, Node &node);

class problem{
    std::vector<std::pair<int, int>> OPERATORS = {{0,-1}, {0,1}, {-1,0}, {1,0}};
    bool GOAL-TEST(const Node& node);
}

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
        int gn = 0;
        int hn = 0;
        std::vector<int> data;
    };


#endif // AL_H