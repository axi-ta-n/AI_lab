#include<bits/stdc++.h>
using namespace std;

struct Node {
    int id;
    int cost;  
};


struct CompareNode {
    bool operator()(Node const& n1, Node const& n2) {
         return n1.cost > n2.cost;
    }
};

// Beam search function
vector<int> beam_search(const vector<vector<int>>& graph, int beam_width) {
    int n = graph.size();
    vector<bool> visited(n, false);
    priority_queue<Node, vector<Node>, CompareNode> pq;
    vector<int> result;

    pq.push({0, 0});
    visited[0] = true;

    while (!pq.empty()) {
        int nodes_at_current_level = min(beam_width, (int)pq.size());
        vector<Node> candidates;

        
        for (int i = 0; i < nodes_at_current_level; ++i) {
            Node current = pq.top();
            pq.pop();
            result.push_back(current.id);

          
            for (int j = 0; j < n; ++j) {
                if (graph[current.id][j] == 1 && !visited[j]) {
                    visited[j] = true;
                    pq.push({j, j});  
                }
            }
        }

        while (pq.size() > beam_width) {
            pq.pop();
        }
    }

    return result;
}

int main() {
    // Sample Graphs
    vector<vector<int>> graph_10_node = {
        {0, 1, 1, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 1, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1, 0, 0, 1, 0}
    };

    vector<vector<int>> graph_12_node = {
        {0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0}
    };

    vector<int> beam_widths = {1, 2, 3};

    cout << "Beam Search on 10-Node Graph:" << endl;
    for (int width : beam_widths) {
        vector<int> result = beam_search(graph_10_node, width);
        cout << "Beam Width " << width << ": ";
        for (int node : result) {
            cout << node << " ";
        }
        cout << endl;
    }

  
    cout << "Beam Search on 12-Node Graph:" << endl;
    for (int width : beam_widths) {
        vector<int> result = beam_search(graph_12_node, width);
        cout << "Beam Width " << width << ": ";
        for (int node : result) {
            cout << node << " ";
        }
        cout << endl;
    }

    return 0;
}
