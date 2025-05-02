// This header file containes all the class and function declarations
#ifndef AL_H
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <queue>
#include <functional>
#include <cmath>
#include <algorithm>
#include <unordered_set>

enum class SearchType {
    Uniform_Cost,
    AStar_Manhattan,
    Astar_Missplaced_Tile,
};

class Node {
    public:
        Node();
        Node(const std::vector<int>& data, int gen = 0);
        Node(const Node& other, std::pair<int, int> move, int gen);
        Node& operator=( const Node& other) ;

        int get_manhattan_distance();
        int get_misplaced_tiles();
        std::vector<int> get_state() const;
        bool operator==(const Node& other) const;
    
        bool operator<(const Node& other) const;
        bool operator>(const Node& other) const;
        void print() const;
    
        int get_manhattan_distance() const { return hn; }
        int get_misplaced_tiles() const { return hn; }
        
        int get_hn() const;
        int get_gn() const;
        int get_cost() const;
        int get_dim() const;
        int get_size() const;
    
       void set_hn(int hn);
        void set_gn(int gn);
        std::string get_state_string() const;

        // Getters for private members
      
     
    
    private:

        int dim = 3;
        int gn = 0;
        int hn = 0;
        std::vector<int> data;
    };


// Node comparator for priority queue
struct NodeComparator {
    bool operator()(const Node& a, const Node& b) {
        return a > b;
    }
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
        return *a > *b; // Assumes Node has operator> defined
    }
};

class Problem{
public:
    Problem(std::vector<int> init_value);
    std::vector<std::pair<int, int>> OPERATORS = {{0,-1}, {0,1}, {-1,0}, {1,0}};
    bool GOAL_TEST(const Node& node);
    const Node& get_initial_state() const { return initial_state; }
    int get_dim() const;
private:
    Node initial_state;
    Node goal_state;
    int dim = 0;
    
  
};

// general search function
// This function takes a problem and a queueing function as input.
int GeneralSearch(Problem& problem,
    std::function<void(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator>&,
                       const std::vector<std::shared_ptr<Node>>&, SearchType)> queueing_function,
    SearchType type);
                  // Node comparator for priority queue

void QueueingFunction(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator>& frontier,
    const std::vector<std::shared_ptr<Node>>& new_nodes, SearchType type);



#endif // AL_H