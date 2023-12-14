#include <unordered_set>
#include "graph.h"
#include "mpi.h"
#include <algorithm>
#include <iostream>
#include <random>


class LubyMPIGraph : public Graph {
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

        int pid;
        int nproc;

        // Create a copy of graph with sets
        std::unordered_map<vertex, std::unordered_set<vertex>> check_graph;
        for (const auto &v : graph) {
            std::unordered_set<vertex> adj_list(graph[v.first].begin(), graph[v.first].end());
            check_graph[v.first] = adj_list;
        }

        // Initialize MPI
        MPI_Init(NULL, NULL);
        // Get process rank
        MPI_Comm_rank(MPI_COMM_WORLD, &pid);
        // Get total number of processes specificed at start of run
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);

        std::cout << "MPI rank: " << pid << std::endl;
        std::cout << "MPI size: " << nproc << std::endl;
        Ind = {};
        Ind.clear(); // Clear the independent set
        std::cout << graph.size() << std::endl;

        // Calculate range of vertices in subgraph
        std::vector<int> count(nproc);
        std::cout << count[0] << std::endl;
        int remainder = graph.size() % nproc;
        std::cout << "remainder: " << remainder << std::endl;
        for (int j = 0; j < nproc; j++) {
            count[j] = graph.size() / nproc;
            if (remainder > 0) {
                count[j]++;
                remainder--;
            }
            std::cout << "iter: " << j << std::endl;
        }
        std::cout << "63" << std::endl;
        std::vector<int> displs(nproc);
        displs[0] = 0;
        for (int j = 1; j < nproc; j++) {
            displs[j] = displs[j-1] + count[j-1];
        }

        // Build subgraph
        std::unordered_map<vertex, std::vector<vertex>> sub_g;
        vertex start_vertex = displs[pid];
        vertex end_vertex = displs[pid] + count[pid];
        for (vertex v = start_vertex; v < end_vertex; v++) {
            sub_g[v] = graph[v];
        }

        std::cout << "76" << std::endl;

        // Run Luby's algorithm on each subgraph
        while (!sub_g.empty()) {
            std::unordered_set<int> S;

            // Parallelize the random selection of vertices
            for (auto i : sub_g) {
                // Check if v should be in S with probability of 1/deg(v)
                vertex v = i.first;
                if ((float)(rand()) / (float)(RAND_MAX) < 1.0 / sub_g[v].size()) {
                    S.insert(v);
                }
            }

            // Parallelize the removal of vertices based on degree comparison
            for (const vertex &v : S) {
                for (const vertex &u : sub_g[v]) {
                    if (sub_g.find(u) != sub_g.end() &&
                        S.find(u) != S.end() &&
                        sub_g[u].size() <= sub_g[v].size()) {
                        S.erase(u);
                    }
                }
            }

            // Remove S and neighbor(S) from graph
            // Insert S to Ind
            for (const vertex &v : S) {
                Ind.insert(v);
                for (const vertex &neighbor : sub_g[v]) {
                    if (sub_g.find(neighbor) != sub_g.end()) {
                        sub_g.erase(neighbor);
                    }
                }
                sub_g.erase(v);
            }
        }



        // Gather subgraphs
        // Count subgraph size
        std::vector<vertex> sub_ind(Ind.begin(), Ind.end());
        std::cout << "117" << std::endl;
        // Gather the size of indset found in each subgraph
        int indset_size = sub_ind.size();
        std::vector<int> recvbuf(nproc);
        recvbuf.resize(nproc);
        std::vector<int> indset_count(nproc);
        std::vector<int> displs_count(nproc);
        for (int i = 0; i < nproc; i++) {
            indset_count[i] = 1;
            displs_count[i] = i;
        }
        std::cout << "128" << std::endl;
        MPI_Allgatherv(&indset_size, 1, MPI_INT, recvbuf.data(), indset_count.data(), displs.data(), MPI_INT, MPI_COMM_WORLD);
        std::cout << "138" << std::endl;

        // Gather the subgraphs
        std::vector<vertex> gathered_list;
        std::vector<int> total_displs(nproc);
        total_displs[0] = 0;
        for (int i = 1; i < nproc; i++) {
            total_displs[i] = recvbuf[i - 1] + total_displs[i - 1];
        }
        std::cout << "147" << std::endl;
        gathered_list.resize(total_displs[nproc - 1] + recvbuf[nproc - 1] + 1);
        MPI_Gatherv(sub_ind.data(), sub_ind.size(), MPI_INT, gathered_list.data(), recvbuf.data(), total_displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
        std::cout << "149" << std::endl;
        // Run Luby's algorithm on the gathered graph
        // Parallelize the removal of vertices based on degree comparison
        std::unordered_set<int> S_all(gathered_list.begin(), gathered_list.end());
        for (const vertex &v : S_all) {
            for (const vertex &u : S_all) {
                if (check_graph[v].find(u) != check_graph[v].end() &&
                    check_graph[u].size() <= check_graph[v].size()) {
                    S_all.erase(u);
                }
            }
        }

        // Remove S_all and neighbor(S_all) from check_graph
        // Insert to Ind_all
        std::unordered_set<vertex> Ind_all;
        Ind_all.clear(); // Clear the independent set
        for (const vertex &v : S_all) {
            Ind_all.insert(v);
            for (const vertex &neighbor : check_graph[v]) {
                if (check_graph.find(neighbor) != check_graph.end()) {
                    check_graph.erase(neighbor);
                }
            }
            check_graph.erase(v);
        }


        while (!check_graph.empty()) {
            std::unordered_set<int> S;

            // Parallelize the random selection of vertices
            for (auto i : check_graph) {
                // Check if v should be in S with probability of 1/deg(v)
                vertex v = i.first;
                if ((float)(rand()) / (float)(RAND_MAX) < 1.0 / check_graph[v].size()) {
                    S.insert(v);
                }
            }

            // Parallelize the removal of vertices based on degree comparison
            for (const vertex &v : S) {
                for (const vertex &u : check_graph[v]) {
                    if (check_graph.find(u) != check_graph.end() &&
                        S.find(u) != S.end() &&
                        check_graph[u].size() <= check_graph[v].size()) {
                        S.erase(u);
                    }
                }
            }

            // Remove S and neighbor(S) from graph
            // Insert S to Ind
            for (const vertex &v : S) {
                Ind_all.insert(v);
                for (const vertex &neighbor : check_graph[v]) {
                    if (check_graph.find(neighbor) != check_graph.end()) {
                        check_graph.erase(neighbor);
                    }
                }
                check_graph.erase(v);
            }
        }

        MPI_Finalize();
        return 0; // Return 0 to indicate success
    }
};

std::unique_ptr<Graph> createLubyMPIGraph() {
    return std::make_unique<LubyMPIGraph>();
}
