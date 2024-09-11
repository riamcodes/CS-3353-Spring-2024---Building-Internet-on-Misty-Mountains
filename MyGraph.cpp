#include <iostream>
#include "MyGraph.h"

// Overload the << operator to print Link objects
ostream& operator<<(ostream& os, const Link& l) {
    os << l.v1 << " " << l.v2 << " " << l.w;
    return os;
}

// Constructor takes in parameter of vertices
MyGraph::MyGraph(int vertices) {
    numVertices = vertices; // Initialize the number of vertices in the graph
}

// Copy constructor
MyGraph::MyGraph(const MyGraph& graphCopy) {
    numVertices = graphCopy.numVertices; // Copy the number of vertices
    edges = graphCopy.edges; // Copy the edges
}

// edgeExist Method checks if an edge already exists in the graph
bool MyGraph::edgeExists(int a, int b) {
    // Go through vector of all edges
    for (const auto& edge : edges) {
        // Check if the edge matches the given vertices (order doesn't matter so check either side with OR)
        if ((edge.v1 == a && edge.v2 == b) || (edge.v1 == b && edge.v2 == a)) {
            return true; // Edge exists
        }
    }
    return false; // Edge does not exist
}

// addEdge bool Method to add a new edge to the graph
bool MyGraph::addEdge(int a, int b, float w) {
    // Check if Vertices are valid
    if ((a < 1 || a > numVertices) || (b < 1 || b > numVertices)) {
        return false; // Vertices are out of bounds
    }
    // Check for an existing edge
    if (edgeExists(a, b)) {
        return false; // Edge already exists, don't add it again
    }
    // Add the new edge
    Link newEdge = {a, b, w};

    edges.push_back(newEdge);
    return true; // Edge successfully added
}

// output Method to output the graph to a given stream
void MyGraph::output(ostream& os) {
    // Print the total number of vertices
    os << numVertices << endl;
    // Iterate through the edges and print them
    for (const auto& edge : edges) {
        // vertex with the smaller number is printed first
        if (edge.v1 < edge.v2) {
            os << edge.v1 << " " << edge.v2 << " " << edge.w;
        } else {
            os << edge.v2 << " " << edge.v1 << " " << edge.w;
        }
        os << endl;
    }
}

// findRoot Method to find the root of a set in unioned sets
int findRoot(int i, vector<int>& parent) {
    // Merge and find root
    while (i != parent[i]) {
        parent[i] = parent[parent[i]];
        i = parent[i];
    }
    return i;
}

// unionSet Method to union two sets in the union structure
void unionSet(int a, int b, vector<int>& parent, vector<int>& rank) {
    int rootA = findRoot(a, parent);
    int rootB = findRoot(b, parent);
    // Union by rank
    if (rank[rootA] < rank[rootB]) {
        parent[rootA] = rootB;
    } else if (rank[rootB] < rank[rootA]) {
        parent[rootB] = rootA;
    } else {
        parent[rootB] = rootA;
        rank[rootA]++;
    }
}

// Method to calculate and return the Minimum Spanning Tree (MST) of the graph using only Kruskal's
// Decided not to implement Prim's algo because the graph should be sparse
pair<vector<pair<int, int>>, float> MyGraph::MST() {
    vector<pair<int, int>> mstEdges; // edges of the MST stored
    float totalWeight = 0.0;         // total weight of the MST stored
    // Sort the edges by weight allows for MINIMUM spanning tree to evaluate min path
    sort(edges.begin(), edges.end(), [](const Link& a, const Link& b) {
        return a.w < b.w;
    });
    // Make union structure to keep track and prevent cycles
    vector<int> parent(numVertices + 1), rank(numVertices + 1, 0);
    iota(parent.begin(), parent.end(), 0);
    // Go through sorted edges and add them to the MST if they DO NOT form a cycle
    for (const auto& edge : edges) {
        //find and store roots to detect if they make a cycle
        int rootA = findRoot(edge.v1, parent);
        int rootB = findRoot(edge.v2, parent);
        //if the roots are not the same then they don't make a cycle, so they can be added to the MST
        if (rootA != rootB) {
            mstEdges.push_back({edge.v1, edge.v2});
            totalWeight += edge.w;
            //final union to finish off MST and return
            unionSet(rootA, rootB, parent, rank);
        }
    }
    return make_pair(mstEdges, totalWeight); // Return the MST edges and their total weight
}

// create AdjListFunction to create an adjacency list from the edges of the graph
vector<vector<int>> createAdjList(const vector<Link>& edges, int n) {
    vector<vector<int>> adjList(n + 1); // +1 for 1-based indexing
    for (const auto& edge : edges) {
        //store adjacent path in one direction
        adjList[edge.v1].push_back(edge.v2);
        //since undirected store adjacent in opposite direction
        adjList[edge.v2].push_back(edge.v1);
    }
    return adjList;
}

// Depth First Search function to find a path from one vertex to another tried to make it Recursive for speed
void dfs(int current, int specifiedNode, const vector<vector<int>>& adjList, vector<bool>& visited, vector<int>& path, bool& found) {
    // Base case: If the path to the specified Node has already been found return to break the recursive call.
    if (found) return;
    // Mark the current node as visited to prevent revisiting it in future recursive calls.
    visited[current] = true;
    // Add the current node to the path. If the specified node is found path will be in order of nodes traversed
    path.push_back(current);
    // Check if the current node IS the specified Node. If so, mark the path as found and return to backtrack the recursion stack.
    if (current == specifiedNode) {
        found = true;
        return;
    }
    // Iterate through all vertices adjacent to the current vertex.
    for (int next : adjList[current]) {
        // If the next vertex has not been visited, recursively call dfs to explore further from that vertex.
        if (!visited[next]) {
            dfs(next, specifiedNode, adjList, visited, path, found);
            // If the path has been found in a recursive call further down, terminate the current call to unwind the recursion stack quickly.
            if (found) return;
        }
    }
    // If this point is reached and the path has not been found, the current vertex does not lead to the specified Node. Remove it from the path.
    // This step is for backtracking
    //undoes the addition of the current node to the path since it's not part of the correct path to the specifiedNode.
    if (!found) path.pop_back();
}

// Main function to process graph data and find paths
pair<vector<Link>, vector<int>> Prog2(vector<float> satcost, vector<Link> linkcost, int& sat_conn) {
    // Step 1: Find the house with the lowest cost for satellite installation
    //Since all the houses are connected anyway we can do this
    sat_conn = distance(satcost.begin(), min_element(satcost.begin() + 1, satcost.end()));

    // Step 2: Find the MST using earlier MST function from earlier (Kruskal's Algorithm)
    int n = satcost.size() - 1; // Number of houses
    MyGraph graph(n + 1); // Assuming vertices are 1-indexed
    for (const auto& link : linkcost) {
            graph.addEdge(link.v1, link.v2, link.w); // Add all edges to the graph
    }

    auto [mstEdgePairs, totalWeight] = graph.MST(); // Use MST() method to get MST edges and total weight

    // Convert edge pairs to Link objects to be stored
   vector<Link> mstEdges;
    for (const auto& edgePair : mstEdgePairs) {
        auto it = find_if(linkcost.begin(), linkcost.end(), [&](const Link& link) {
            // Check if the current link corresponds to the current edgePair, considering both possible orders
            return (link.v1 == edgePair.first && link.v2 == edgePair.second) ||
                   (link.v1 == edgePair.second && link.v2 == edgePair.first);
        });
        if (it != linkcost.end()) {
            // Create a new Link object based on the found iterator
            Link newEdge = *it;
            // Swap v1 and v2 if they are not in order to ensure newEdge.v1 < newEdge.v2
            if (newEdge.v1 > newEdge.v2) {
                std::swap(newEdge.v1, newEdge.v2);
            }
            // Add the ink object to the vector of MST edges
            mstEdges.push_back(newEdge);
        }
    }
    // Step 3: Path from satellite to house 1 stored in second vector
    vector<vector<int>> adjList = createAdjList(mstEdges, n);
    // Find the path from the satellite connected house to house 1
    vector<bool> visited(n + 1, false);
    vector<int> path;
    bool found = false;
    dfs(sat_conn, 1, adjList, visited, path, found);

    // Add 0 in beginning to represent the satellite connection in the path
    path.insert(path.begin(), 0);
    return {mstEdges, path}; // Return the MST edges and the path
}
