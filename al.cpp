#include"al.h"

Node::Node() {
    this->dim = 3;
    this->gn = 0;
    this->hn = 0;
    this->data.resize(this->dim * this->dim);
    for (int i = 0; i < this->dim * this->dim; i++) {
        this->data[i] = i+1;
    }
    this->data[this->dim * this->dim - 1] = 0; 
}

Node::Node(const std::vector<int>& data, int gen) {
    this->data = data;
    this->gn = gen;
    this->hn = 0;
    this->dim = static_cast<int>(std::sqrt(data.size()));
}

Node::Node(const Node& other, std::pair<int, int> move, int gen) {
    this->data = other.data;
    this->gn = gen;
    this->hn = 0;
    this->dim = other.dim;
    // Move the zero tile, translate 1d to 2d to operate
    int zero_index = std::find(this->data.begin(), this->data.end(), 0) - this->data.begin();
    int zero_row = zero_index / this->dim;
    int zero_col = zero_index % this->dim;
    int new_row = zero_row + move.first;
    int new_col = zero_col + move.second;
    if (new_row >= 0 && new_row < dim && new_col >= 0 && new_col < dim) {
        int new_zero_index = new_row * dim + new_col;
        std::swap(this->data[zero_index], this->data[new_zero_index]);
    } else {
        throw std::invalid_argument("Invalid move");
    }
}

std::vector<int> Node::get_state() const {
    return this->data;
}
bool Node::operator==(const Node& other) const {
    return this->data == other.data;
}

Node& Node::operator=( const Node& other) {
    if (this != &other) { // Protect against self-assignment
        this->data = other.data;
        this->gn = other.gn;
        this->hn = other.hn;
        this->dim = other.dim;
        // Copy other members as necessary
    }
    return *this;
}

bool Node::operator<(const Node& other) const {
    return (this->gn + this->hn) < (other.gn + other.hn);
}

bool Node::operator>(const Node& other) const {
    return (this->gn + this->hn) > (other.gn + other.hn);
}

void Node::print() const {
    for (int i = 0; i < this->dim; ++i) {
        for (int j = 0; j < this->dim; ++j) {
            std::cout << this->data[i * this->dim + j] << " ";
        }
        std::cout << std::endl;
    }
}

// Print out the path from root to this node
void Node::print_parent() const {
    std::vector<std::shared_ptr<Node>> path;
    if (this->parent) {
        auto current = this->parent;
        while (current) {
            path.push_back(current);
            current = current->parent;
        }
        std::reverse(path.begin(), path.end());
        for (const auto& node : path) {
            node->print();
            std::cout << "Depth: " << node->get_gn() << std::endl;
        }
    } 
}

// Calculates the Manhattan distance for the current node
int Node::get_manhattan_distance() {
    int distance = 0;
    for (int i = 0; i < this->data.size(); ++i) {
        int val = this->data[i];
        if (val != 0) { // Skip the blank tile
            int goal_row = (val - 1) / this->dim;
            int goal_col = (val - 1) % this->dim;
            int curr_row = i / this->dim;
            int curr_col = i % this->dim;
            distance += std::abs(goal_row - curr_row) + std::abs(goal_col - curr_col);
        }
    }
    return distance;
}

// Calculates the number of misplaced tiles for the current node
int Node::get_misplaced_tiles() {
    // simple add up
    int count = 0;
    for (int i = 0; i < this->data.size(); ++i) {
        if (this->data[i] != 0 && this->data[i] != i+1) {
            count++;
        }
    }
    return count;
}

int Node::get_cost() const {
    return this->gn + this->hn;
}

int Node::get_dim() const {
    return this->dim;
}

int Node::get_size() const {
    return this->data.size();
}

int Node::get_gn() const {
    return this->gn;
}

int Node::get_hn() const {
    return this->hn;
}

void Node::set_hn(int hn){
    this->hn = hn;
}

void Node::set_gn(int gn){
    this->gn = gn;
}

std::string Node::get_state_string() const{
    std::string state_str;
    for (const auto& val : this->data) {
        state_str += std::to_string(val) + ",";
    }
    return state_str;
}

Problem::Problem(std::vector<int> init_value) {
    // We assume it is a square
    this->dim = std::sqrt(init_value.size());
    if (this->dim * this->dim != init_value.size()) {
        throw std::invalid_argument("Invalid initial state size");
    }
    this->initial_state = Node(init_value,0);
    // 0 at the back
    std::vector<int> goal_data(init_value.size(),0);
    for (int i = 0; i < init_value.size()-1; i++) {
        goal_data[i] = i+1;
    }
    this->goal_state = Node(goal_data,0);
    // Initialize goal state 0 to n-1
   
}

// Check if the current node is the goal state
bool Problem::GOAL_TEST(const Node& node) {
    return node.get_state() == this->goal_state.get_state();
}

int Problem::get_dim() const {
    return this->dim;
}

//This is the core of the program
int GeneralSearch(Problem& problem,
    std::function<void(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator>&,
                       const std::vector<std::shared_ptr<Node>>&, SearchType)> queueing_function,
    SearchType type){
    
    // metrics
    int expanded_nodes = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    // This is effective the make queue
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> frontier;
    std::unordered_set<std::string> explored;
    auto start_node = std::make_shared<Node>(problem.get_initial_state());
    std::cout << "Initial state:"<<std::endl;
    start_node->print();
    explored.insert(start_node->get_state_string());
    frontier.push(start_node);

    // Do the search, the empy clause is outside the loop
    while (!frontier.empty()) {

        //effective: node = REMOVE-FRONT(nodes) 
        auto current_node = frontier.top();
        frontier.pop();
        std::cout<<"Current Cost: "<<current_node->get_cost()<<std::endl;
        expanded_nodes++;
        
        //effective: if problem.GOAL-TEST(node.STATE) succeeds then return node 
        if (problem.GOAL_TEST(*current_node)) {
            auto end_time = std::chrono::high_resolution_clock::now();
            std::cout << "Goal state reached!" << std::endl;
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            std::cout << "Time taken: " << duration.count() << " ms"<< std::endl;
            std::cout << "Expanded nodes: " << expanded_nodes << std::endl;
            std::cout << "Solution found with depth and cost: " << current_node->get_gn()<<", "<< current_node->get_cost() << std::endl;
            std::cout << "Solution path:"<<std::endl;
            current_node->print_parent();
            return current_node->get_cost();
        }

        std::cout << "Exploring node with depth: " << current_node->get_gn() << std::endl;
        current_node->print();
        // Expand node
        std::vector<std::shared_ptr<Node>> successors;
        for (const auto& op : problem.OPERATORS) {
            try {
                auto child = std::make_shared<Node>(*current_node, op, current_node->get_gn() + 1);
                if (explored.find(child->get_state_string()) == explored.end()) {
                    successors.push_back(child);
                    child->set_parent(current_node);
                    explored.insert(child->get_state_string());
                }
            } catch (const std::invalid_argument& e) {
                continue;
            }
        }

        //effective:  nodes = QUEUEING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))
        queueing_function(frontier, successors, type);

    }
    // Fail state
    std::cout << "Failure: No solution found."<<std::endl;
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time taken: " << duration.count() << " ms"<<std::endl;
    return -1;
}

//Calculate the heuristic value for each node and add it to the priority queue
void QueueingFunction(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator>& frontier,
    const std::vector<std::shared_ptr<Node>>& new_nodes, SearchType type){
    // Note that gn is calculated when creation.

    for (const auto& node : new_nodes) {
        if (type == SearchType::Uniform_Cost) {
            node->set_hn(0);
            frontier.push(node);
        } else if (type == SearchType::AStar_Manhattan) {
            node->set_hn (node->get_manhattan_distance());
            frontier.push(node);
        } else if (type == SearchType::Astar_Missplaced_Tile) {
            node->set_hn(node->get_misplaced_tiles());
            frontier.push(node);
        }
    }
}