#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <stack>

using std::unordered_map;
using std::unordered_set;
using std::set;
using std::queue;
using std::stack;
using std::string;

class Graph {
public:
    unordered_map<string, set<string>> neighbors;
    
    void add_edge(const string& from, const string& to, const char& graph_type) {
        neighbors[from].insert(to);
        if (graph_type == 'u')
            neighbors[to].insert(from);
    }
    
    void width_first_search(const string& start_v) {
        queue<string> queue_of_vertices;
        unordered_set<string> visited;
        queue_of_vertices.push(start_v);
        visited.insert(start_v);
        while(!queue_of_vertices.empty()) {
            string current_v = queue_of_vertices.front();
            queue_of_vertices.pop();
            std::cout << current_v << "\n";
            for (auto neighbour : neighbors[current_v]){
                if (visited.find(neighbour) == visited.end()) {
                    queue_of_vertices.push(neighbour);
                    visited.insert(neighbour);
                }
            }
        }
    }
    
    void depth_first_search(const string& start_v) {
        stack<string> stack_of_vertices;
        unordered_set<string> visited;
        stack_of_vertices.push(start_v);
        string current_v;
        visited.insert(current_v);
        while(!stack_of_vertices.empty()) {
            current_v = stack_of_vertices.top();
            stack_of_vertices.pop();
            if (visited.find(current_v) == visited.end()) {
                std::cout << current_v << "\n";
                visited.insert(current_v);
                for (auto it = neighbors[current_v].rbegin(); it != neighbors[current_v].rend(); ++it) {
                    if (visited.find(*it) == visited.end()) {
                        stack_of_vertices.push(*it);
                    }
                }
            }
        }
    }
};

int main() {
    Graph graph;
    char graph_type, search_type;
    string start_vertex;
    std::cin >> graph_type >> start_vertex >> search_type;
    string from, to;
    while (!std::cin.eof()) {
        std::cin >> from >> to;
        graph.add_edge(from, to, graph_type);
    }
    if (search_type == 'b')
        graph.width_first_search(start_vertex);
    else if (search_type == 'd')
        graph.depth_first_search(start_vertex);
}
