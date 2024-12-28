// KRUSKAL'S MINIMUM SPANNING TREE (MST) ALGORITHM

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <queue>
using namespace std;

string fileName = "graph.txt";

struct Connection {
    int from, to, cost;
};

struct Graph {
    int numVertices = 0;
    vector<Connection*> edges;
};

void displayMenu() {
    cout << endl;
    cout << "Kruskal's Algorithm - Options:" << endl;
    cout << "1) Load graph from file" << endl;
    cout << "2) Generate a random graph" << endl;
    cout << "3) Run Kruskal's algorithm" << endl;
    cout << "4) Display accepted connections" << endl;
    cout << "5) Exit" << endl;
    cout << endl;
    cout << "Choice: ";
}

void loadGraph(Graph& G) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error: Could not open the file '" << fileName << "'. Please check if the file exists and is accessible." << endl;
        return;
    }

    int numVertices, numEdges;
    if (!(file >> numVertices >> numEdges)) {
        cerr << "Error: Failed to read the number of vertices and edges from the file. Ensure the file format is correct." << endl;
        file.close();
        return;
    }

    G.edges.clear();        // in case we already generated graph before
    G.numVertices = numVertices;

    int fromNode, toNode, cost;
    int lineNumber = 1;

    while (file >> fromNode >> toNode >> cost) {
        lineNumber++;
        if (fromNode <= 0 || toNode <= 0 || fromNode > numVertices || toNode > numVertices) {
            cerr << "Error: Invalid node indices on line " << lineNumber
                << ". Nodes must be between 1 and " << numVertices << "." << endl;
            continue; 
        }

        Connection* connection = new Connection();
        connection->from = fromNode;
        connection->to = toNode;
        connection->cost = cost;
        G.edges.push_back(connection);
    }

    if (G.edges.empty()) {
        cerr << "Error: No valid edges were read from the file. Check the file content and format." << endl;
    }
    else {
        cout << "Graph loaded successfully with " << G.numVertices << " vertices and "
            << G.edges.size() << " edges." << endl;
    }

    file.close();
}

void generateGraph(Graph& G, int numVertices) {
    G.edges.clear();        // in case we already generated/read graph before
    G.numVertices = numVertices;

    for (int i = 0; i < (numVertices * (numVertices - 1) / 2); i++) { // Max number of edges in an undirected graph: n(n-1)/2
        Connection* connection = new Connection();
        connection->from = (rand() % numVertices + 1);
        connection->to = (rand() % numVertices + 1);

        while (connection->from == connection->to ||
            (find_if(G.edges.begin(), G.edges.end(), [&](Connection* existing) {
                return (existing->from == connection->to && existing->to == connection->from) ||
                    (existing->from == connection->from && existing->to == connection->to);
                }) != G.edges.end())) {
            connection->from = (rand() % numVertices + 1);
            connection->to = (rand() % numVertices + 1);
        }
        connection->cost = (rand() % numVertices + 1);
        G.edges.push_back(connection);
    }
    cout << endl;
}

bool isConnected(const Graph& G) {
    vector<bool> visited(G.numVertices + 1, false);
    queue<int> q;
    int visitedCount = 0;

    q.push(1);
    visited[1] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        visitedCount++;

        for (auto& edge : G.edges) {
            if (edge->from == current && !visited[edge->to]) {
                visited[edge->to] = true;
                q.push(edge->to);
            }
            else if (edge->to == current && !visited[edge->from]) {
                visited[edge->from] = true;
                q.push(edge->from);
            }
        }
    }

    return visitedCount == G.numVertices;
}


int partition(vector<Connection*>& edges, int low, int high) {
    int pivot = edges[low]->cost;
    int left = low;
    int right = high;

    while (left < right) {
        while (edges[left]->cost <= pivot && left < high)
            left++;
        while (edges[right]->cost >= pivot && right > low)
            right--;

        if (left < right)
            swap(edges[left], edges[right]);
    }

    swap(edges[low], edges[right]);
    return right;
}

void quickSort(vector<Connection*>& edges, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(edges, low, high);
        quickSort(edges, low, pivotIndex - 1);
        quickSort(edges, pivotIndex + 1, high);
    }
}

int findNextSet(vector<int>& sets) {
    int maxSet = 0;
    for (int value : sets) {
        if (maxSet < value)
            maxSet = value;
    }
    return maxSet + 1;
}

vector<Connection*> runKruskal(Graph& G) {
    auto start = std::chrono::steady_clock::now();
    // Step 1: Sort the edges of the graph in ascending order by cost
    quickSort(G.edges, 0, G.edges.size() - 1);

    vector<Connection*> result;                 // Stores the edges included in the minimum spanning tree
    vector<int> parent(G.numVertices + 1, 0);   // Tracks the parent of each node for union-find

    // Lambda to find the root of a node
    auto find = [&parent](int node) {
        while (parent[node] != 0) {             // Traverse up the parent chain until the root is reached
            node = parent[node];
        }
        return node;                            
    };

    // Lambda to unite two sets 
    auto unite = [&parent, &find](int node1, int node2) {
        parent[find(node2)] = find(node1);      // Connect the root of one set to the root of the other
    };

    // Step 2: Process edges in increasing order of cost
    for (auto& edge : G.edges) {
        // Find the sets (roots) of the two nodes connected by this edge
        int set1 = find(edge->from);
        int set2 = find(edge->to);

        if (set1 != set2) {                     // If nodes are in different sets, no cycle is formed
            result.push_back(edge);             // Include this edge in the result
            unite(set1, set2);                  // Merge the two sets
        }

        // If we've included enough edges to connect all vertices, we can stop
        if (result.size() == G.numVertices - 1) {
            break;
        }
    }

    // Step 3: Check if we have a valid solution
    if (result.size() != G.numVertices - 1) {
        // If the number of edges in the result is not (number of vertices - 1),
        // the graph is disconnected => no minimum spanning tree exists
        cout << "No solution found! The graph might be disconnected." << endl;
        return {};
    }

    auto end = std::chrono::steady_clock::now();
    cout << "Kruskal's algorithm completed successfully!" << endl;
    cout << "Execution time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "μs" << endl;
    return result;
}

void printGraph(const Graph& G) {
    for (Connection* edge : G.edges) {
        cout << "from: " << edge->from << " to: " << edge->to << " cost: " << edge->cost << endl;
    }
    cout << endl;
}

void printAcceptedConnections(const vector<Connection*>& result) {
    for (Connection* edge : result) {
        cout << "from: " << edge->from << " to: " << edge->to << " cost: " << edge->cost << endl;
    }
    cout << endl;
}

int main() {
    int choice;
    bool isRunning = true;

    Graph G, G1, G2;
    int numVertices;

    vector<Connection*> result;

    do {
        displayMenu();
        cin >> choice;
        system("cls");
        switch (choice) {
        case 1:
            loadGraph(G1);
            printGraph(G1);
            G = G1;
            break;
        case 2:
            cout << "Number of vertices in the graph: ";
            cin >> numVertices;
            generateGraph(G2, numVertices);
            G = G2;
            break;
        case 3:
            if (!isConnected(G)) {
                cout << "Error: The graph is not connected. Kruskal's algorithm cannot find a solution." << endl;
                break;
            }
            result = runKruskal(G);
            break;
        case 4:
            printAcceptedConnections(result);
            break;
        case 5:
            isRunning = false;
            break;
        default:
            cout << "Invalid option!" << endl;
            break;
        }
    } while (isRunning);

    return 0;
}