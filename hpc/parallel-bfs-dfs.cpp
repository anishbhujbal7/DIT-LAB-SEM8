#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Add undirected edge
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // ---------------- DFS ----------------
    void parallelDFSUtil(int v, vector<bool> &visited) {
        visited[v] = true;

        #pragma omp critical
        cout << v << " ";

        for (int i = 0; i < adj[v].size(); i++) {
            int n = adj[v][i];

            if (!visited[n]) {
                #pragma omp task
                parallelDFSUtil(n, visited);
            }
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);

        #pragma omp parallel
        {
            #pragma omp single
            parallelDFSUtil(start, visited);
        }
    }

    // ---------------- BFS ----------------
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int size = q.size();

            // Process one level at a time
            #pragma omp parallel for
            for (int i = 0; i < size; i++) {

                int node;

                #pragma omp critical
                {
                    node = q.front();
                    q.pop();
                }

                #pragma omp critical
                cout << node << " ";

                for (int j = 0; j < adj[node].size(); j++) {
                    int neighbor = adj[node][j];

                    if (!visited[neighbor]) {
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }
    }
};

// ---------------- MAIN ----------------
int main() {
    Graph g(7);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    cout << "Parallel DFS: ";
    g.parallelDFS(0);

    cout << "\nParallel BFS: ";
    g.parallelBFS(0);

    return 0;
}