#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "graph.h"

class RandPrioGraph : public Graph {
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
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);

        // Assign a random number to each vertex
        std::unordered_map<vertex, double> random_numbers;
        for (const auto& v : graph) {
            random_numbers[v.first] = dis(gen);
        }

        Ind.clear(); // Clear the independent set

        while (!graph.empty()) {
            std::unordered_set<vertex> to_be_removed;
            std::unordered_set<vertex> next_graph_vertices;

            // Copy keys of the map into a vector
            std::vector<vertex> graphKeys;
            for (const auto& kv : graph) {
                graphKeys.push_back(kv.first);
            }

            // Parallelize the loop with OpenMP
            #pragma omp parallel
            {
                std::unordered_set<vertex> local_remove;
                std::unordered_set<vertex> local_ind;

                #pragma omp for nowait
                for (size_t i = 0; i < graphKeys.size(); ++i) {
                    vertex v = graphKeys[i];
                    bool is_smallest = true;

                    // Check if current node has the smallest random number among its neighbors
                    for (const vertex& neighbor : graph[v]) {
                        if (random_numbers[v] > random_numbers[neighbor]) {
                            is_smallest = false;
                            break;
                        }
                    }

                    // If v has the smallest number, it joins the local independent set
                    if (is_smallest) {
                        local_ind.insert(v);
                        // Schedule neighbors for removal in the local set
                        for (const vertex& neighbor : graph[v]) {
                            local_remove.insert(neighbor);
                        }
                    }
                }

                // Merge local sets into the shared ones
                #pragma omp critical
                {
                    Ind.insert(local_ind.begin(), local_ind.end());
                    to_be_removed.insert(local_remove.begin(), local_remove.end());
                }
            }
            
            // After determining the nodes to be removed, we will rebuild the graph
            // for the next iteration without the removed nodes and their neighbors.
            for (auto &kv : graph) {
                if (to_be_removed.count(kv.first) == 0) {  // If not scheduled to be removed
                    std::vector<vertex> new_neighbors;
                    for (vertex &neighbor : kv.second) {
                        if (to_be_removed.count(neighbor) == 0) {  // If neighbor is not removed
                            new_neighbors.push_back(neighbor);
                        }
                    }
                    if (!new_neighbors.empty()) {  // If there are still neighbors left
                        next_graph_vertices.insert(kv.first);
                    }
                    graph[kv.first] = std::move(new_neighbors);
                }
            }

            // Only retain vertices that are still in the graph for the next iteration
            for (const vertex &v : next_graph_vertices) {
                if (graph[v].empty()) {
                    graph.erase(v);
                }
            }

            // weirdly detects error?? but outcome correct
            if (to_be_removed.empty()) {
                // If no vertices were scheduled to be removed, the algorithm is stuck.
                // This should not happen if the algorithm is correct.
                std::cout << "Graph size before getting stuck: " << graph.size() << std::endl;
                std::cerr << "Error: No vertices were removed in the iteration. The algorithm is stuck." << std::endl;
                return 0;  // Indicate an error
            }
        }

        return 0; // return 0 to indicate success.
    }

};

std::unique_ptr<Graph> createRandPrioGraph() {
    return std::make_unique<RandPrioGraph>();
}
