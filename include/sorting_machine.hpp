/* 
sorting_machine.hpp
Author: Dialo Sall

*/
#ifndef SORTING_MACHINE_HPP
#define SORTING_MACHINE_HPP

#include "graph.hpp"
#include <vector>

namespace threecolor {

// Bucket for a specific Degree
struct Bucket {
    int degree; // static degree value represented by this bucket
    Vertex* head; //doubly-linked list of vertices with this degree
};

class SortingMachine {
public:
    SortingMachine();

    // Build buckets from graph degrees; assumes graph.degrees computed
    void initialize(Graph& g);

    // Empty check (meaning no uncolored vertices left)
    bool empty() const;

    // Insert v into appropriate bucket, ordered by colored_neighbors
    void insert(Vertex* v);

    // Remove v from its bucket
    void remove(Vertex* v);

    // Update when neighbor of v gets colored
    void bump_colored_neighbors(Vertex* v);

    // Pick next vertex to color (Highest degree, then most colored_neighbors)
    Vertex* pick_vertex();

private:
    Graph* graph;              // Pointer to the graph
    std::vector<int> degree_values; // Unique degree values in graph, sorted descending
    std::vector<Bucket> buckets;    // Buckets 
    std::vector<int> degree_to_bucket; // Map degree to bucket index or -1

    int find_bucket_index_for_degree(int degree) const;
};
} // namespace threecolor

#endif // SORTING_MACHINE_HPP
