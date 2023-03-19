#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED
#include <cstddef>
#include <vector>
#include <tuple>

class Graph
{

  private:
    struct Edge {
        size_t endpoint;
        size_t capacity;
    };
    size_t src_;
    size_t sink_;
    size_t n;
    std::vector<Edge> *adjList_;


  public:
    Graph(size_t numVertices, size_t src, size_t sink);
    Graph(const Graph &other);
    ~Graph();
    void addEdge(size_t start, size_t end, size_t capacity);
    void print();

    // vector<Edge>& getEdges(size_t vertex);
    void changeCapacity(size_t start, size_t end, size_t newCapacity);

    std::vector<size_t> findAugmentingPath();
    size_t fordFulkerson();
};

#endif