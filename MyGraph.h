//
// Created by riamu on 3/23/2024.
//

#ifndef PROGRAM2_MYGRAPH_H
#define PROGRAM2_MYGRAPH_H

#include <iostream>
#include <vector>
#include <algorithm> // For std::sort
#include <numeric>   // For std::iota

using namespace std;

// Link created by Dr.Lin meant as an edge data structure
struct Link {
public:
    int v1, v2; // Vertices of the edge
    float w;    // Weight of the edge
};

//printing of Link objects
ostream& operator<<(ostream&, const Link&);

// MyGraph Class
class MyGraph {
public:
    MyGraph(int vertices); // Constructor with parameter of vertices
    MyGraph(const MyGraph&); // Copy constructor

    bool addEdge(int a, int b, float w); // Add an edge to the graph
    void output(ostream& os); // Output the graph to an ostream
    pair<vector<pair<int, int>>, float> MST(); // Calculate the Minimum Spanning Tree (MST) of the graph using Kruskal's Algorithm

private:
    int numVertices; // Number of vertices in the graph
    vector<Link> edges; // Vector of link objects holds all links or edges in the graph
    bool edgeExists(int a, int b); // Check if an edge exists between two vertices
    // NOTE: findRoot, dfs, and unionSet helper functions are used internally but not declared in the header
};

// Put everything together function
pair<vector<Link>, vector<int>> Prog2(vector<float> satcost, vector<Link> linkcost, int& sat_conn);

#endif //PROGRAM2_MYGRAPH_H
