#include <unordered_set>
#include "graph.h"
#include "omp.h"
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

    int buildIndSet(std::unordered_map<vertex, std::vector<vertex>>& graph,
                    std::unordered_set<vertex>& Ind) {
        Ind.clear(); // Clear the independent set

        // Initialize thread-local random number generators
        int maxThreads = omp_get_max_threads();
        std::vector<std::mt19937> rngs(maxThreads);
        std::random_device rd;
        for (int i = 0; i < maxThreads; ++i) {
            rngs[i].seed(rd());
        }

        while (!graph.empty()) {
            std::unordered_set<vertex> S;

            // Copy keys of the map into a vector
            std::vector<vertex> graphKeys;
            for (const auto& kv : graph) {
                graphKeys.push_back(kv.first);
            }

            // Parallelize the random selection of vertices
            #pragma omp parallel
            {
                std::vector<vertex> localS;
                int thread_id = omp_get_thread_num();
                std::uniform_real_distribution<float> dist(0.0, 1.0);

                #pragma omp for nowait
                for (size_t i = 0; i < graphKeys.size(); ++i) {
                    vertex v = graphKeys[i];
                    if (dist(rngs[thread_id]) < 1.0 / graph[v].size()) {
                        localS.push_back(v);
                    }
                }

                #pragma omp critical
                {
                    for (vertex v : localS) {
                        S.insert(v);
                    }
                }
            }

            // Apply the removals outside of the parallel region
            std::unordered_set<vertex> toRemove;
            for (vertex v : S) {
                for (vertex u : graph[v]) {
                    if (graph.find(u) != graph.end() && S.find(u) != S.end() && graph[u].size() <= graph[v].size()) {
                        toRemove.insert(u);
                    }
                }
            }
            for (vertex v : toRemove) {
                S.erase(v);
            }

            // Remove S and neighbor(S) from graph and insert S into Ind
            for (vertex v : S) {
                Ind.insert(v);
                // Store neighbors before erasing to avoid iterator invalidation
                std::vector<vertex> neighbors(graph[v].begin(), graph[v].end());
                for (vertex neighbor : neighbors) {
                    graph.erase(neighbor);
                }
                graph.erase(v);
            }
        }

        return 0; // Return 0 to indicate success
    }

};

std::unique_ptr<Graph> createLubyGraph() {
    return std::make_unique<LubyGraph>();
}