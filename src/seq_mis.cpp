#include <unordered_set>
#include "graph.h"


class SeqGraph : public Graph {
public:
    void buildGraph(std::vector<vertex> &vertices, 
                    std::vector<std::pair<int, int>> &edges,
                    std::unordered_map<vertex, std::vector<vertex>> &graph) {
        //initialize Neighbors in G for each v 
        for (auto &vertex : vertices) {
            graph[vertex] = {};
        }

        //load info from E to G's Neighbors
        for (auto &edge : edges) {
            graph[edge.first].push_back(edge.second);
            graph[edge.second].push_back(edge.first);
        }
    }

    int buildIndSet(std::unordered_map<vertex, std::vector<vertex>> &graph,
                    std::unordered_set<vertex> &Ind) {
        Ind = {};
        Ind.clear(); // Clear the independent set

        while (!graph.empty()) {
            // Choose a node v from the graph
            auto it = graph.begin();
            vertex v = it->first;
            Ind.insert(v); // Add v to the independent set

            // Remove v and all its neighbors from the graph
            // for all neighbors of v, if present in G, remove it
            for (const vertex &neighbor : graph[v]) {
                if (graph.find(neighbor) != graph.end()) {
                    graph.erase(neighbor);
                }
            }
            graph.erase(v);
        }

        return 0; // Return 0 to indicate success
    }
};

std::unique_ptr<Graph> createSeqGraph() {
    return std::make_unique<SeqGraph>();
}