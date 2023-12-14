#include <unordered_set>
#include "graph.h"
#include "omp.h"
#include <algorithm>
#include <iostream>
#include <random>


class LubyTaskGraph : public Graph {
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
        std::random_device rd;
        std::vector<std::mt19937> rngs(omp_get_max_threads());
        for (auto& rng : rngs) {
            rng.seed(rd());
        }

        while (!graph.empty()) {
            std::unordered_set<vertex> S;

            // Copy keys of the map into a vector for easy access
            std::vector<vertex> graphKeys(graph.size());
            size_t idx = 0;
            for (const auto& kv : graph) {
                graphKeys[idx++] = kv.first;
            }

            // Define chunk size for tasks (adjust this based on the graph size and desired granularity)
            size_t chunkSize = graphKeys.size() / 10 + 1;

            #pragma omp parallel
            {
                #pragma omp single
                {
                    for (size_t i = 0; i < graphKeys.size(); i += chunkSize) {
                        #pragma omp task firstprivate(i)
                        {
                            std::vector<vertex> localS;
                            std::uniform_real_distribution<float> dist(0.0, 1.0);
                            int thread_id = omp_get_thread_num();

                            size_t end = std::min(i + chunkSize, graphKeys.size());
                            for (size_t j = i; j < end; ++j) {
                                vertex v = graphKeys[j];
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
                        } // end of task
                    }
                } // end of single
            } // end of parallel

            // Apply removals outside of the parallel region
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
                std::vector<vertex> neighbors(graph[v].begin(), graph[v].end());
                for (vertex neighbor : neighbors) {
                    graph.erase(neighbor);
                }
                graph.erase(v);
            }

            // Check if the graph is empty and break if needed
            if (graph.empty()) {
                break;
            }
        }

        return 0; // Return 0 to indicate success
    }

};

std::unique_ptr<Graph> createLubyTaskGraph() {
    return std::make_unique<LubyTaskGraph>();
}
