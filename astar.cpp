#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>
#include <limits>

using namespace std;

class Graph {
private:
    map<char, map<char, int>> graph;
    map<char, int> hValues;

    // Utility structure for priority queue
    struct Node {
        char vertex;
        int cost;
        vector<char> path;
        
        Node(char v, int c, vector<char> p) : vertex(v), cost(c), path(p) {}
        
        bool operator>(const Node& other) const {
            return cost > other.cost;
        }
    };

public:
    // Add edge to the graph
    void addEdge(char from, char to, int cost) {
        graph[from][to] = cost;
    }

    // Add heuristic value
    void addHValue(char node, int value) {
        hValues[node] = value;
    }

    // 1. Depth First Search
    vector<char> dfs(char start) {
        set<char> visited;
        vector<char> path;
        stack<char> s;
        
        s.push(start);
        
        while (!s.empty()) {
            char current = s.top();
            s.pop();
            
            if (visited.find(current) == visited.end()) {
                visited.insert(current);
                path.push_back(current);
                
                vector<pair<char, int>> neighbors;
                for (const auto& neighbor : graph[current]) {
                    neighbors.push_back(neighbor);
                }
                sort(neighbors.rbegin(), neighbors.rend());
                
                for (const auto& neighbor : neighbors) {
                    if (visited.find(neighbor.first) == visited.end()) {
                        s.push(neighbor.first);
                    }
                }
            }
        }
        return path;
    }

    // 2. Best First Search
    vector<char> bestFirstSearch(char start, char goal) {
        priority_queue<Node, vector<Node>, greater<Node>> pq;
        set<char> visited;
        
        pq.push(Node(start, 0, vector<char>{start}));
        
        while (!pq.empty()) {
            Node current = pq.top();
            pq.pop();
            
            if (current.vertex == goal) {
                return current.path;
            }
            
            if (visited.find(current.vertex) != visited.end()) {
                continue;
            }
            
            visited.insert(current.vertex);
            
            for (const auto& neighbor : graph[current.vertex]) {
                if (visited.find(neighbor.first) == visited.end()) {
                    vector<char> newPath = current.path;
                    newPath.push_back(neighbor.first);
                    pq.push(Node(neighbor.first, neighbor.second, newPath));
                }
            }
        }
        return vector<char>();
    }

    // 3. Iterative Deepening
    vector<char> iterativeDeepening(char start, char goal, int maxDepth = 10) {
        for (int depth = 0; depth <= maxDepth; depth++) {
            set<char> visited;
            vector<char> result = depthLimitedSearch(start, goal, depth, visited);
            if (!result.empty()) {
                return result;
            }
        }
        return vector<char>();
    }

    // 4. Breadth First Search
    vector<char> bfs(char start) {
        queue<char> q;
        set<char> visited;
        vector<char> path;
        
        q.push(start);
        visited.insert(start);
        
        while (!q.empty()) {
            char current = q.front();
            q.pop();
            path.push_back(current);
            
            for (const auto& neighbor : graph[current]) {
                if (visited.find(neighbor.first) == visited.end()) {
                    visited.insert(neighbor.first);
                    q.push(neighbor.first);
                }
            }
        }
        return path;
    }

    // 5. Beam Search
    vector<char> beamSearch(char start, char goal, int beamWidth = 2) {
        vector<Node> beam = {Node(start, hValues[start], vector<char>{start})};
        set<char> visited;
        
        while (!beam.empty()) {
            vector<Node> nextBeam;
            
            for (const auto& current : beam) {
                if (current.vertex == goal) {
                    return current.path;
                }
                
                if (visited.find(current.vertex) != visited.end()) {
                    continue;
                }
                
                visited.insert(current.vertex);
                
                for (const auto& neighbor : graph[current.vertex]) {
                    if (visited.find(neighbor.first) == visited.end()) {
                        vector<char> newPath = current.path;
                        newPath.push_back(neighbor.first);
                        nextBeam.push_back(Node(neighbor.first, hValues[neighbor.first], newPath));
                    }
                }
            }
            
            sort(nextBeam.begin(), nextBeam.end(), 
                 [](const Node& a, const Node& b) { return a.cost < b.cost; });
                 
            beam.clear();
            for (int i = 0; i < min(beamWidth, (int)nextBeam.size()); i++) {
                beam.push_back(nextBeam[i]);
            }
        }
        return vector<char>();
    }

    // 6. A* Search
    vector<char> aStarSearch(char start, char goal) {
        priority_queue<Node, vector<Node>, greater<Node>> pq;
        map<char, int> gScore;
        set<char> visited;
        
        for (const auto& node : graph) {
            gScore[node.first] = numeric_limits<int>::max();
        }
        
        gScore[start] = 0;
        pq.push(Node(start, hValues[start], vector<char>{start}));
        
        while (!pq.empty()) {
            Node current = pq.top();
            pq.pop();
            
            if (current.vertex == goal) {
                return current.path;
            }
            
            if (visited.find(current.vertex) != visited.end()) {
                continue;
            }
            
            visited.insert(current.vertex);
            
            for (const auto& neighbor : graph[current.vertex]) {
                char next = neighbor.first;
                int newGScore = gScore[current.vertex] + neighbor.second;
                
                if (newGScore < gScore[next]) {
                    gScore[next] = newGScore;
                    vector<char> newPath = current.path;
                    newPath.push_back(next);
                    pq.push(Node(next, newGScore + hValues[next], newPath));
                }
            }
        }
        return vector<char>();
    }

private:
    vector<char> depthLimitedSearch(char node, char goal, int depth, set<char>& visited) {
        if (depth < 0) return vector<char>();
        if (node == goal) return vector<char>{node};
        
        visited.insert(node);
        
        for (const auto& neighbor : graph[node]) {
            if (visited.find(neighbor.first) == visited.end()) {
                vector<char> path = depthLimitedSearch(neighbor.first, goal, depth - 1, visited);
                if (!path.empty()) {
                    path.insert(path.begin(), node);
                    return path;
                }
            }
        }
        
        visited.erase(node);
        return vector<char>();
    }
};

// Utility function to print path
void printPath(const vector<char>& path) {
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << endl;
}

int main() {
    Graph g;
    
    // Add edges
    g.addEdge('A', 'B', 6); g.addEdge('A', 'C', 3); g.addEdge('A', 'D', 1);
    g.addEdge('B', 'C', 2); g.addEdge('B', 'D', 6); g.addEdge('B', 'E', 3); g.addEdge('B', 'F', 4);
    g.addEdge('C', 'E', 4); g.addEdge('C', 'F', 5);
    g.addEdge('D', 'E', 7); g.addEdge('D', 'F', 8); g.addEdge('D', 'G', 9);
    g.addEdge('E', 'F', 6); g.addEdge('E', 'G', 9); g.addEdge('E', 'H', 9);
    g.addEdge('F', 'G', 8); g.addEdge('F', 'H', 9);
    g.addEdge('G', 'H', 11); g.addEdge('G', 'I', 12); g.addEdge('G', 'J', 14);
    g.addEdge('H', 'I', 14); g.addEdge('H', 'J', 15);
    g.addEdge('I', 'J', 0);
    
    // Add h-values
    g.addHValue('A', 15); g.addHValue('B', 13); g.addHValue('C', 13); g.addHValue('D', 12);
    g.addHValue('E', 10); g.addHValue('F', 9); g.addHValue('G', 7); g.addHValue('H', 6);
    g.addHValue('I', 5); g.addHValue('J', 0);
    
    cout << "1. Depth First Search:\n";
    printPath(g.dfs('A'));
    
    cout << "\n2. Best First Search:\n";
    printPath(g.bestFirstSearch('A', 'J'));
    
    cout << "\n3. Iterative Deepening:\n";
    printPath(g.iterativeDeepening('A', 'J'));
    
    cout << "\n4. Breadth First Search:\n";
    printPath(g.bfs('A'));
    
    cout << "\n5. Beam Search (Width=2):\n";
    printPath(g.beamSearch('A', 'J', 2));
    
    cout << "\n6. A* Search:\n";
    printPath(g.aStarSearch('A', 'J'));
    
    return 0;
}
