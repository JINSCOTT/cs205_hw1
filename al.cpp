#include"al.h"
Node::Node() {
    this->dim = 3;
    this->gn = 0;
    this->hn = 0;
    this->data.resize(this->dim * this->dim);
    for (int i = 0; i < this->dim * this->dim; i++) {
        this->data[i] = i;
    }
}
Node::Node(const std::vector<int>& data, int gen) {
    this->data = data;
    this->gn = gen;
    this->hn = 0;
}

Node::Node(const Node& other, std::pair<int, int> move, int gen) {
    this->data = other.data;
    this->gn = gen;
    this->hn = 0;
    // We assume the move is valid
    int zero_index = std::find(this->data.begin(), this->data.end(), 0) - this->data.begin();
    int new_zero_index = zero_index + move.first * dim + move.second;
    std::swap(this->data[zero_index], this->data[new_zero_index]);
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

int Node::get_manhattan_distance() {
    int distance = 0;
    for (int i = 0; i < this->data.size(); ++i) {
        if (this->data[i] != 0) {
            int goal_row = this->data[i] / this->dim;
            int goal_col = this->data[i] % this->dim;
            int current_row = i / this->dim;
            int current_col = i % this->dim;
            distance += std::abs(goal_row - current_row) + std::abs(goal_col - current_col);
        }
    }
    return distance;
}
int Node::get_misplaced_tiles() {
    int count = 0;
    for (int i = 0; i < this->data.size(); ++i) {
        if (this->data[i] != 0 && this->data[i] != i) {
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
 
    std::vector<int> goal_data(init_value.size());
    for (int i = 0; i < init_value.size(); i++) {
        goal_data[i] = i;
    }
    this->goal_state = Node(goal_data,0);
    // Initialize goal state 0 to n-1
   
}

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
    
    // This is effective the make queue
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> frontier;
    std::unordered_set<std::string> explored;
    auto start_node = std::make_shared<Node>(problem.get_initial_state());
    frontier.push(start_node);

    while (!frontier.empty()) {
        auto current_node = frontier.top();
        frontier.pop();

        
        // Goal test
        if (problem.GOAL_TEST(*current_node)) {
            auto end_time = std::chrono::high_resolution_clock::now();
            std::cout << "Solution found with cost: " << current_node->get_cost() << "\n";
            current_node->print(); // Assuming this prints the path or state
            return current_node->get_cost();
        }

        std::cout << "Exploring node with cost: " << current_node->get_cost() << "\n";
        current_node->print();

        explored.insert(current_node->get_state_string());

        // Expand node
        std::vector<std::shared_ptr<Node>> successors;
        for (const auto& op : problem.OPERATORS) {
            auto child = std::make_shared<Node>(*current_node, op, current_node->get_gn() + 1);
            if (explored.find(child->get_state_string()) == explored.end()) {
                successors.push_back(child);
            }
        }

        // Apply the queueing function
        queueing_function(frontier, successors, type);
    }

    std::cout << "Failure: No solution found.\n";
    return -1;



}

void QueueingFunction(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator>& frontier,
    const std::vector<std::shared_ptr<Node>>& new_nodes, SearchType type){

    for (const auto& node : new_nodes) {
        if (type == SearchType::Uniform_Cost) {
            // We already added cost when creating the node
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