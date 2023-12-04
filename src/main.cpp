#include "graph.h"
#include "timing.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>


// implementation versions
enum class MISType { Sequential, LubyOpenMP};

struct StartupOptions {
    std::string inputFile = "";
    MISType type = MISType::Sequential;
};

StartupOptions parseOptions(int argc, const char **argv) {
    StartupOptions so;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            so.inputFile = argv[i+1];
        } else if (strcmp(argv[i], "-seq") == 0) {
            so.type = MISType::Sequential;
        } else if (strcmp(argv[i], "-lubyopenmp") == 0) {
            so.type = MISType::LubyOpenMP;
        }
        // else if (strcmp(argv[i], "-openmp") == 0) {
        //     so.type = MISType::OpenMP;
        // } else if (strcmp(argv[i], "-jpop") == 0) {
        //     so.type = MISType::JPOpenMP;
        // } else if (strcmp(argv[i], "-half") == 0) {
        //     so.type = MISType::HalfJP;
        // }
    }
    return so;
}

bool checkCorrectness(std::vector<vertex> &vertices,
                      std::unordered_map<vertex, std::vector<vertex>> &orig_graph,
                      std::unordered_set<vertex> &Ind) {
    
    // Check for independence
    for (const vertex &v : Ind) {
        auto it = orig_graph.find(v);
        if (it != orig_graph.end()) {
            for (const vertex &neighbor : it->second) {
                if (Ind.count(neighbor)) {
                    // Found a neighbor in Ind, not independent
                    return false;
                }
            }
        }
    }

    // Check for maximality
    for (const vertex &v : vertices) {
        if (Ind.count(v) == 0) { // If v is not in Ind
            bool hasNeighborInInd = false;
            auto it = orig_graph.find(v);
            if (it != orig_graph.end()) {
                for (const vertex &neighbor : it->second) {
                    if (Ind.count(neighbor)) {
                        hasNeighborInInd = true;
                        break; // At least one neighbor is in Ind
                    }
                }
            }
            if (!hasNeighborInInd) {
                // Found a vertex not in Ind and without a neighbor in Ind
                return false;
            }
        }
    }

    return true; // Passed both independence and maximality checks

}

bool readGraphFromFile(std::string fileName, std::vector<vertex> &vertices,
                            std::vector<std::pair<vertex, vertex>> &edges) {
    std::ifstream inFile;

    inFile.open(fileName);
    if (!inFile) {
        return false;
    }

    std::string line;

    std::getline(inFile, line);
    std::stringstream sstream(line);
    std::string str;
    std::getline(sstream, str, '\n');
    int numVertices = (int) atoi(str.c_str());

    for (int i = 0; i < numVertices; i++) {
        vertices.push_back(i);
    }

    while(std::getline(inFile, line)) {
        std::stringstream sstream2(line);
        std::getline(sstream2, str, ' ');
        int v1 = (int) atoi(str.c_str());
        std::getline(sstream2, str, '\n');
        int v2 = (int) atoi(str.c_str());

        edges.push_back(std::make_pair(v1, v2));
    }

    return true;
}

void createCompleteTest(std::vector<vertex> &vertices,
                        std::vector<std::pair<vertex, vertex>> &edges) {
    int numVertices = 5000;
    vertices.resize(numVertices);
    for (int i = 0; i < numVertices; i++) {
        vertices[i] = i;
    }
    edges.clear();
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            edges.push_back(std::make_pair(i, j));
        }
    }
}

int main(int argc, const char **argv) {
    StartupOptions options = parseOptions(argc, argv);

    std::vector<vertex> vertices;
    std::vector<std::pair<vertex, vertex>> edges;
    std::cout << options.inputFile << std::endl;
    readGraphFromFile(options.inputFile, vertices, edges);
    if (!readGraphFromFile(options.inputFile, vertices, edges)) { // if fail to read from file, arbitrarily create test 
        createCompleteTest(vertices, edges);
        std::cout << "Failed to read graph from input file\n";
    }

    std::unique_ptr<Graph> g;

    switch (options.type) {
        case MISType::Sequential:
            g = createSeqGraph();
            break;
        case MISType::LubyOpenMP:
            g = createLubyGraph();
            break;
        // case MISType::OpenMP:
        //     cg = createOpenMPColorGraph();
        //     break;
        // case MISType::JPOpenMP:
        //     cg = createJPOpenMPColorGraph();
        //     break;
        // case MISType::HalfJP:
        //     cg = createHalfJPOpenMPColorGraph();
        //     break;

    }

    std::unordered_map<vertex, std::vector<vertex>> orig_graph;
    std::unordered_map<vertex, std::vector<vertex>> temp_graph;
    std::unordered_set<vertex> indSet;
    g->buildGraph(vertices, edges, orig_graph);

    Timer t;
    g->buildGraph(vertices, edges, temp_graph);
    t.reset();
    g->buildIndSet(temp_graph, indSet);

    double time_spent = t.elapsed();
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(5);
    std::cout << "Time spent: " << time_spent << std::endl;

    // Print information about indSet
    std::cout << "Size of Independent Set: " << indSet.size() << std::endl;

    // Print a few elements from the set
    std::cout << "Some elements from the Independent Set: ";
    int count = 0;
    const int maxElementsToShow = 10; // Change this number to show more or fewer elements
    for (const auto& element : indSet) {
        std::cout << element << " ";
        if (++count >= maxElementsToShow) {
            break;
        }
    }
    if (indSet.size() > maxElementsToShow) {
        std::cout << "... (and more)";
    }
    std::cout << std::endl;

    if (!checkCorrectness(vertices, orig_graph, indSet)) {
        std::cout << "Failed to find MIS correctly\n";
        return -1;
    }
    else {
        std::cout << "Found MIS correctly\n";
    }

    return 0;
}