#include "graph.hpp"
#include <iostream>
#include <list>
#include <algorithm>
#include <limits>
#include <fstream>
#include <string>
//Hippeas organic chickpea puffs

using namespace std;


Graph::Graph(size_t n, size_t src, size_t sink):
src_{src}, sink_{sink}, n{n},adjList_{new vector<Edge>[n]}
{
    //nothing? 
}

Graph::~Graph()
{
    delete[] adjList_;
}

// Copy constructor
Graph::Graph(const Graph& other):
    src_{other.src_}, sink_{other.sink_}, n{other.n},
    adjList_{new vector<Edge>[other.n]} {
    for (size_t i = 0; i < n; ++i)
    {
        adjList_[i] = other.adjList_[i];
    }
}

void Graph::addEdge(size_t start, size_t end, size_t capacity){
    adjList_[start].push_back(Edge{end, capacity});
}

// void Graph::reduceCapacity(size_t start, size_t end, size_t reduce){
//     // Undefined behavior for edge not in graph
//     for (Edge e : adjList_[start]){
//         if (e.endpoint == end){
//             e.capacity = newCapacity;
//         }
//     }
// }

void Graph::print(){
    cout << "Src: " << src_ << " Sink: " << sink_ << endl;
    for (size_t v = 0; v < n; ++v){
        cout << v << ": ";
        for (Edge edge : adjList_[v])
        {
            cout << "(" << edge.endpoint << ", " << edge.capacity << ") ";
        }
        cout << endl;
    }
}

vector<size_t> Graph::findAugmentingPath(){
    
    list<size_t> q;
    // Establish Visited nodes and set up backtracking
    vector<bool> visited(n);
    fill(begin(visited), end(visited), false);
    size_t *addedFrom = new size_t[n];
    fill(addedFrom, addedFrom + n, std::numeric_limits<size_t>::infinity());
    
    // Set up for while loop
    addedFrom[src_] = src_;
    q.push_back(src_);
    while (q.size() > 0){
        size_t curNode = q.front();
        q.pop_front();
        for (Edge e : adjList_[curNode]){
            if (!visited[e.endpoint]){
                q.push_back(e.endpoint);
                visited[e.endpoint] = true;
                addedFrom[e.endpoint] = curNode;
            }
        }
    }
    // Backtracking
    vector<size_t> path;
    size_t curNode = sink_;
    // Check if there is an augmenting path. 
    if (addedFrom[sink_] != numeric_limits<size_t>::infinity()){
       while (curNode != src_){
            path.push_back(curNode);
            curNode = addedFrom[curNode];
       }
       path.push_back(src_);
       std::reverse(begin(path), end(path));
    }
    delete[] addedFrom;
    return path;
}

size_t Graph::fordFulkerson(){
    // find starting min flow;
    size_t startMinFlow;
    for (size_t v = 0; v < n; ++v)
    {
       for (Edge e : adjList_[v]){
            startMinFlow = max(startMinFlow, e.capacity);
       }
    }
    size_t flow = 0;
    while(true){
       vector<size_t> path = findAugmentingPath();
       if (path.size() == 0) {
            break;
        }
        
        size_t minFlow = startMinFlow;
        vector<size_t> edgeInds;
        for (size_t pathInd = 0; pathInd < path.size() - 1; ++pathInd)
        {
            size_t start = path[pathInd];
            size_t edgeInd = 0;
            for (Edge e : adjList_[start]){
                if (e.endpoint == path[pathInd+1]){
                    minFlow = min(e.capacity, minFlow);
                    edgeInds.push_back(edgeInd);
                }
                ++edgeInd;
            }
        }
        flow += minFlow;
        for (auto x : path){
            cout << x << " ";
        }
        cout << "Flow: " << minFlow << endl;

        // Save index of vertex to delete
        size_t emptyVert, emptyInd;
        for (size_t i = 0; i < path.size() - 1; ++i){
            size_t start = path[i];
            size_t end = path[i + 1];
            adjList_[start][edgeInds[i]].capacity -= minFlow;
            if (minFlow == 0){
                emptyVert = start;
                emptyInd = i;
            }
            // Add residual Edges
            bool edgeExists = false;
            for (Edge edge : adjList_[end]){
                if (edge.endpoint == start){
                    edgeExists = true;
                    edge.capacity += minFlow;
                }
            }
            if (!edgeExists){
                addEdge(end, start, minFlow);
            }
        }

        // Delete edge with no flow remaining;
        cout << "erasing vertex " << emptyVert << " endpoint " << adjList_[emptyVert][edgeInds[emptyInd]].endpoint << endl;
        adjList_[emptyVert].erase(begin(adjList_[emptyVert]) + edgeInds[emptyInd]);
        print();
    }
    return flow;
}

Graph readFromFile(std::string filename){
    //numVert, numEdges, src, sink
    ifstream inputfile(filename);
    size_t numVertices, numEdges, src, sink;
    inputfile >> numVertices;
    inputfile >> numEdges;
    inputfile >> src;
    inputfile >> sink;
    Graph g{numVertices, src, sink};
    for (size_t i = 0; i < numEdges; ++i){
        size_t start, end, capacity;
        inputfile >> start;
        inputfile >> end;
        inputfile >> capacity;
        g.addEdge(start, end, capacity);
    }
    return g;
}

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "No file given" << endl;
        return 1;
    }
    Graph g = readFromFile(argv[1]);
    // g.print();
    size_t maxFlow = g.fordFulkerson();
    cout << maxFlow << endl;
    return 0;
}