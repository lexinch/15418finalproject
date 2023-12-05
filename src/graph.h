#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

typedef int vertex;

class Graph {
    public:
        
        virtual void buildGraph(std::vector<vertex> &vertices,
                                std::vector<std::pair<vertex, vertex>> &edges,
                                std::unordered_map<vertex, std::vector<vertex>> &graph) = 0;

        virtual int buildIndSet(std::unordered_map<vertex, std::vector<vertex>> &graph,
                    std::unordered_set<vertex> &Ind) = 0;

};

std::unique_ptr<Graph> createSeqGraph();
std::unique_ptr<Graph> createLubyGraph();
std::unique_ptr<Graph> createRandPrioGraph();