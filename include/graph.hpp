/*
graph.hpp
Author: Dialo Sall


*/
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
#include <cstdint>

namespace threecolor {
    enum Color : int {
        RED = 0,
        BLUE = 1,
        YELLOW = 2,
        UNCOLORED = -1
    };

    struct Vertex;

    struct Neighbor {
        Vertex* v;
        Neighbor* next;
    };

    struct Vertex {
        int id;     // 0...n-1
        int degree; // static degree in graph
        Color color;    //RED/BLUE/YELLOW/UNCOLORED

        int colored_neighbors; // number of colored neighbors
        uint8_t forbidden_mask; // bit i=1 if color i is forbidden
        bool is_trapped; // true if exactly 1 color is available

        // adjacency list
        Neighbor* neighbors_head;

        //links used by SortingMachine buckets
        Vertex* next_in_bucket;
        Vertex* prev_in_bucket;

        Vertex();
    };

    class Graph {
    public:
        Graph() = default;

        explicit Graph(int n);

        int num_vertices() const { return static_cast<int>(vertices.size()); }

        Vertex& vertex(int id) { return vertices[id]; }
        const Vertex& vertex(int id) const { return vertices[id]; }

        // Add undirected edge u-v
        void add_edge(int u, int v);

        // Compute degree field for each vertex
        void compute_degrees();

        // Reset dynamic field (Colors, Masks, etc.)
        void reset_coloring_state();

        // Optional: load graph from simple edge list file
        static Graph from_edge_list_file(const std::string& path);

    private:
        std::vector<Vertex> vertices;
    };


} // namespace threecolor

#endif // GRAPH_HPP