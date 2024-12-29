# Kruskal's Algorithm for Minimum Spanning Tree (MST)

This repository contains an implementation of Kruskal's algorithm in C++ to find the Minimum Spanning Tree (MST) of a graph. The program provides functionalities for loading a graph from a file, generating a random graph, and finding the MST using Kruskal's algorithm.

---

## 📚 **Theory: What is Kruskal's Algorithm?**
Kruskal's algorithm is a greedy algorithm used to find the MST of a connected, weighted graph. The MST is a subgraph that:
- Includes all vertices of the original graph.
- Has the minimum possible total edge weight.
- Does not form cycles.

### **Steps of Kruskal's Algorithm:**
1. **Sort:** Sort all edges in ascending order of their weights.
2. **Pick Edges:** Add edges one by one to the MST, ensuring that no cycles are formed.
3. **Stop:** Continue until the MST includes all vertices (with exactly \(n-1\) edges, where \(n\) is the number of vertices).

The algorithm is efficient for sparse graphs and widely used in network optimization problems like designing computer, road, or electrical networks.

---

## 🖥️ **Program Features**

### **1. Load Graph from File**
The program reads a graph from a file named `graph.txt`.
- The file should have the following format:
  ```
  <number_of_vertices>
  <number_of_edges>
  <from_node> <to_node> <cost>
  <from_node> <to_node> <cost>
  ...
  ```
  Example:
  ```
  8
  13
  1 2 1
  2 3 2
  4 6 2
  ...
  ```

### **2. Generate Random Graph**
Generate a random graph with a user-specified number of vertices. The graph is undirected and has randomly assigned edge weights.

### **3. Run Kruskal's Algorithm**
Find the MST of the graph using Kruskal's algorithm. The program ensures that the graph is connected before proceeding.

### **4. Display Results**
View the edges included in the MST along with their weights.

---

## ⚙️ **How to Use**

### **Requirements**
- A C++ compiler supporting C++11 or later.

### **Steps to Run**
1. Clone the repository:
   ```bash
   git clone <repository_url>
   ```
2. Navigate to the project directory:
   ```bash
   cd <repository_directory>
   ```
3. Compile the program:
   ```bash
   g++ -std=c++11 -o kruskal kruskal.cpp
   ```
4. Run the program:
   ```bash
   ./kruskal
   ```

### **Program Menu**
When the program runs, you'll see the following menu:
```
Kruskal's Algorithm - Options:
1) Load graph from file
2) Generate a random graph
3) Run Kruskal's algorithm
4) Display accepted connections
5) Exit
```
Follow the prompts to interact with the program.

---

## 📝 **Sample Output**
### Example Input Graph (`graph.txt`):
```
8
13
1 2 1
2 3 2
4 6 2
3 5 3
7 8 3
1 3 4
...
```
### Output after running Kruskal's algorithm:
```
Kruskal's algorithm completed successfully!
Execution time: 1234μs
Accepted Connections:
from: 1 to: 2 cost: 1
from: 2 to: 3 cost: 2
from: 4 to: 6 cost: 2
...
```

---

## 📂 **File Structure**
```
.
├── graph.txt          # Example graph input file
└── Vaja9.cpp          # Source code
```

