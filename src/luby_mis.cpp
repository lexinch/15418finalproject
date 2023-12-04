#include <unordered_set>
#include "graph.h"
#include <algorithm>
#include <iostream>
#include <random>


class LubyGraph : public Graph {
public:
    void buildGraph(std::vector<vertex> &vertices, 
                    std::vector<std::pair<int, int>> &edges,
                    std::unordered_map<vertex, std::vector<vertex>> &graph) {
        
        // initialize Neighbors in G for each v 
        for (auto &vertex : vertices) {
            graph[vertex] = {};
        }

        // load info from E to G's Neighbors
        for (auto &edge : edges) {
            graph[edge.first].push_back(edge.second);
            graph[edge.second].push_back(edge.first);
        }
    }

    int buildIndSet(std::unordered_map<vertex, std::vector<vertex>> &graph,
                    std::unordered_set<vertex> &Ind) {
        Ind = {};
        Ind.clear(); // Clear the independent set
        
        std::cout << graph.size() << std::endl;
        while (!graph.empty()) {
            std::unordered_set<int> S;

            // Parallelize the random selection of vertices
            #pragma omp parallel for
            for (auto i : graph) {
                // Check if v should be in S with probability of 1/deg(v)
                vertex v = i.first;
                if ((float)(rand()) / (float)(RAND_MAX) < 1.0 / graph[v].size()) {
                    #pragma omp critical
                    S.insert(v);
                }
            }

            // Parallelize the removal of vertices based on degree comparison
            #pragma omp parallel for
            for (const vertex &v : S) {
                for (const vertex &u : graph[v]) {
                    if (graph.find(u) != graph.end() && 
                        S.find(u) != S.end() && 
                        graph[u].size() <= graph[v].size()) {
                        #pragma omp critical
                        S.erase(u);
                    }
                }
            }

            // Remove S and neighbor(S) from graph
            // Insert S to Ind
            #pragma omp parallel for
            for (const vertex &v : S) {
                Ind.insert(v);
                for (const vertex &neighbor : graph[v]) {
                    if (graph.find(neighbor) != graph.end()) {
                        #pragma omp critical
                        graph.erase(neighbor);
                    }
                }
                #pragma omp critical
                graph.erase(v);
            }
        }


        return 0; // Return 0 to indicate success
    }
};

std::unique_ptr<Graph> createLubyGraph() {
    return std::make_unique<LubyGraph>();
}