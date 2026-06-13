# 3ColorProject

An experimental greedy heuristic for the 3-coloring problem, built with a custom **Matrix Sorting Machine** priority structure.

This project explores an approach to graph 3-coloring: instead of coloring vertices in a fixed order, the algorithm dynamically prioritizes vertices using two levels of sorting:

1. Higher-degree vertices are handled first.
2. Within each degree group, vertices with more colored neighbors are prioritized

The solver also detects **trapped vertices**, meaning vertices that only have one valid color option remaining, and colors those vertices before returning to the normal order of coloring using the sorting machine.

This repository does not claim to solve the 3-coloring problem, or prove P=NP. It is a research-focused prototype for visualizing, testing, and improving a greedy strategy for graph coloring. 

---
## Motivation

The graph 3-coloring problem asks whether the vertices of a graph can be colored using only three colors such that no two adjacent vertices share the same color.

This problem is important in graph theory and computer science because it is one of the classic NP-complete problems. A polynomial-time algorithm that correctly solves every instance of graph 3-coloring would have major implications for the P vs NP problem. 

My long-term research goal is to keep exploring whether useful structure can be found in hard coloring instances. This project is one step in that larger direction. The current algorithm is a greedy heuristic, meaning it can find valid 3-colorings for some graphs, but failure does not prove that a graph is impossible to 3-color. 

---
## Unique Approach: Matrix Sorting Machine

The central structure in this project is what I call a **Matrix Sorting Machine**. 

It acts as a two-dimensional priority system:

    - The inner buckets contain vertices of the same degree that are sorted by the number of colored neighbors of each vertex from high to low.
    - The groups of buckets are sorted by the degree of the vertices in them, from high to low.

The first priority level is vertex degree. Vertices with higher degree are considered more important because they have more edges and can affect more of the graph.

The second priority level is the number of colored neighbors. As more of a vertex's neighbors become colored, that vertex becomes more constrained, so it moves forward within its degree bucket.

The solver also has an override rule: if a vertex becomes trapped with only one color available, it is colored before the sorting machine chooses a vertex.

To summarize, the selection order is:

1. Color trapped vertices first.
2. Otherwise, choose from the Matrix Sorting Machine.
3. Within the chosen vertex, assign the first available color using RED, then BLUE, then YELLOW.
4. Update neighboring vertices and repeat

---
## What Success and Failure Indicates

If the algorithm successfully colors a graph and the verification function passes, then the graph is definitely 3-colorable because the program produced a valid coloring.

However, if the algorithm fails, that does not prove the graph is not 3-colorable.

Because the algorithm is greedy, and does not use backtracking, failure can mean either:

1. The graph is not 3-colorable.
2. The graph is 3-colorable, but the greedy choices led it to a dead end

---
## Repository Structure

3ColorProject/
```text
.
├── Makefile
├── README.md
├── include
│   ├── algorithm.hpp
│   ├── graph.hpp
│   ├── sorting_machine.hpp
│   └── utilities.hpp
├── src
│   ├── algorithm.cpp
│   ├── graph.cpp
│   ├── main.cpp
│   ├── sorting_machine.cpp
│   └── utilities.cpp
└── tests
    ├── benchmark.cpp
    └── test_graphs.cpp

```
`src/main.cpp`

The main entry point for the program. It loads a graph from an input file, or generates a random graph if one isn't given. Next, it runs the solver, prints the results, verifies the coloring, and can export the graph to DOT format for visualization.

`src/graph.cpp`

Contains the graph abstraction. It stores vertices, edges, degrees, colors, adjacency information, and the logic for loading graphs.

`src/sorting_machine.cpp`

Implements the Matrix Sorting Machine. This structure organizes uncolored vertices by degree and colored-neighbor count so the solver can choose which vertex to color next.

`src/algorithm.cpp`

Contains the main greedy 3-coloring algorithm. It handles initialization, trapped vertices, color selection, neighbor updates, contradictions, and final verification of 3-coloring.

`src/utilities.cpp`

Contains helper functions for generating random graphs, building small test graphs, exporting graphs to DOT files, and creating random number generators.

`tests/test_graphs.cpp `

Runs small test cases, including a colorable graph and a complete graph on four vertices, which cannot be 3-colored.

`tests/benchmark.cpp`

Runs the solver across random graphs of different sizes and densities to measure success rate and runtime.

---
## Build Instructions

This project uses C++17 and a Makefile.

To build everything:

`make all`

This creates 3 executables:

color3
test_graphs
bench

To clean generated executables and visualization files:

`make clean`

---
## Running the Solver

To run the main solver on a randomly generated graph:

`./color3`

To run the solver on a graph file:

`./color3 path/to/graph.txt`

The graph file should use this format:

```text
number_of_vertices number_of_edges
u1 v1
u2 v2
u3 v3
...
```
Example:
```text
4 6
0 1
0 2
0 3
1 2
1 3
2 3
```
This example is `K4`, the complete graph on four vertices. Since every vertex connects to every other vertex, it is not 3-colorable.

---
## Running tests

To run the small test graphs:

`./test_graphs`

The expected behavior is:

```text
Test graph 1:
    Result: SUCCESS

Test graph 2:
    Result: FAILURE
```

The first graph is designed to be 3-colorable. The second graph is `K4`, which is not 3-colorable.

---
## Running Benchmarks

To run the benchmark suite:

`./bench`

The benchmark tests random graphs across different values using:

```text
n = number of vertices
p = probability that an edge exists between two vertices
```

A larger `p` usually means a denser graph, which creates more coloring constraints, so they are usually harder for a greedy 3-coloring algorithm.

Example benchmark results from one run:

```text
n=20, p=0.02 -> successes=100/100, time=427 us
n=20, p=0.05 -> successes=100/100, time=536 us
n=20, p=0.08 -> successes=98/100, time=639 us
n=20, p=0.1 -> successes=99/100, time=477 us
n=20, p=0.15 -> successes=91/100, time=517 us
n=20, p=0.2 -> successes=43/100, time=860 us
n=40, p=0.02 -> successes=100/100, time=1159 us
n=40, p=0.05 -> successes=100/100, time=1569 us
n=40, p=0.08 -> successes=85/100, time=1907 us
n=40, p=0.1 -> successes=35/100, time=1781 us
n=40, p=0.15 -> successes=0/100, time=2019 us
n=40, p=0.2 -> successes=0/100, time=2219 us
n=60, p=0.02 -> successes=100/100, time=1962 us
n=60, p=0.05 -> successes=92/100, time=2800 us
n=60, p=0.08 -> successes=9/100, time=2734 us
n=60, p=0.1 -> successes=0/100, time=2848 us
n=60, p=0.15 -> successes=0/100, time=3558 us
n=60, p=0.2 -> successes=0/100, time=3869 us
n=80, p=0.02 -> successes=100/100, time=2912 us
n=80, p=0.05 -> successes=35/100, time=3434 us
n=80, p=0.08 -> successes=0/100, time=3723 us
n=80, p=0.1 -> successes=0/100, time=3988 us
n=80, p=0.15 -> successes=0/100, time=4962 us
n=80, p=0.2 -> successes=0/100, time=5621 us
n=100, p=0.02 -> successes=98/100, time=3674 us
n=100, p=0.05 -> successes=0/100, time=4228 us
n=100, p=0.08 -> successes=0/100, time=4993 us
n=100, p=0.1 -> successes=0/100, time=5436 us
n=100, p=0.15 -> successes=0/100, time=6464 us
n=100, p=0.2 -> successes=0/100, time=7995 us
```

These results show that the heuristic performs best on sparse graphs. As graph density increases, the success rate drops intensely.

This is expected. Dense random graphs have more constraints, and many are most likely not 3-colorable. Also, since this algorithm does not backtrack, it may fail on some graphs that are technically 3-colorable.

---
## Visualize a Graph

The project can export a graph to DOT format, which can be converted into an image using Graphviz.

First, run the solver:

`./color3`

If DOT export is enabled in main.cpp, this creates:

`colored_success.dot` or `colored_failure.dot`

Convert it to a PNG:

`dot -Tpng colored_success.dot -o colored_success.png`

or

`dot -Tpng colored_failure.dot -o colored_failure.png`



Open the image:

`open output.png`

Color meaning:

```text
red     = vertex colored RED
blue    = vertex colored BLUE
yellow  = vertex colored YELLOW
gray    = uncolored vertex
```
If the solver succeeds, the visualization should show no gray vertices and no adjacent vertices with the same color.

---
## Algorithm Overview

At a high level, the algorithm functions like this:

```text
1. Reset all vertices to UNCOLORED.
2. Initialize the Matrix Sorting Machine.
3. While uncolored vertices remain:
    a. If a trapped vertex exists, select it.
    b. Otherwise, select the next vertex from the Sorting Machine.
    c. Choose the first available color using RED -> BLUE -> YELLOW priority.
    d. If no color is available, return failure.
    e. Color the vertex.
    f. Update each uncolored neighbor:
        - Mark the chosen color as forbidden.
        - If the neighbor has no colors left, return failure.
        - If the neighbor has only one color left, mark it as trapped.
        - Increase its colored-neighbor count and reposition it in the Sorting Machine.
4. Verify that the final coloring is valid.
```
---
## Current Limitations

This project is still experimental. Current limitations:

```text
- The algorithm is greedy and doesn't backtrack to prevent exponential runtime
- Failure does not prove that the graph is not 3-colorable.
- The RED -> BLUE -> YELLOW color priority may cause avoidable dead ends.
- The success rate drops on larger or denser random graphs(doesn't mean it fails!).
- The benchmark uses random graphs, so results may vary between runs.
```

The most important decision is that the algorithm cannot reverse decisions. Once it colors a vertex, it does not revisit it.

---
## Future Work

Possible next steps:
```text
1. Save failed graphs automatically for later analysis.
2. Compare the Matrix Sorting Machine heuristic against known heuristics like DSATUR.
3. Experiment with different color priority rules.
4. Add a repair step after a contradiction is found.
5. Find graph structures that cause this heuristic to fail.
6. Improve visualizations to show the coloring process step by step
```

The most important future step is counterexample searching; when the greedy algorithm fails, saving that graph and determining whether it is truly 3-colorable or not can tell me what other flaws this algorithm has.

---
## Research Framing

This project should be understood as an exploratory algorithmic prototype.

It does not claim to solve graph 3-coloring in polynomial time. Instead, it is meant to highlight the effects of this heuristic on certain graph families to reveal useful patterns for future algorithms.

This distinction is central for me right now, but this project is only the beginning!

---
## Author

Dialo Sall

Computer Science and Engineering student at The Ohio State University.

This project is part of my broader interest in algorithms, computational complexity, graph theory, and the P vs NP problem.