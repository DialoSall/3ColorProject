/*
graph.cpp
Author: Dialo Sall
*/
#include "graph.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace threecolor {

    Vertex::Vertex()
        : id(-1), degree(0), color(UNCOLORED), colored_neighbors(0), forbidden_mask(0), 
        is_trapped(false), neighbors_head(nullptr), next_in_bucket(nullptr), prev_in_bucket(nullptr) {}

    Graph::Graph(int n) 
        : vertices(n) 
    {
        for (int i = 0; i < n; ++i) {
            vertices[i].id = i;
        }
    }

    void Graph::add_edge(int u, int v) {
        if (u < 0 || v < 0 || u >= num_vertices() || v >= num_vertices()) {
            throw std::out_of_range("Cannot add edge: vertex id out of range");
        }
        if (u == v) return; // no self-loops

        // u to v
        Neighbor* neighbor_u = new Neighbor{ &vertices[v], vertices[u].neighbors_head };
        vertices[u].neighbors_head = neighbor_u;

        // v to u
        Neighbor* neighbor_v = new Neighbor{ &vertices[u], vertices[v].neighbors_head };
        vertices[v].neighbors_head = neighbor_v;
    }

    void Graph::compute_degrees() {
        for (auto& vertex : vertices) {
            int d = 0;
            for (Neighbor* nbr = vertex.neighbors_head; nbr != nullptr; nbr = nbr->next) {
                ++d;
            }
            vertex.degree = d;
        }
    }

    void Graph::reset_coloring_state() {
        for(auto& vertex : vertices) {
            vertex.color = UNCOLORED;
            vertex.colored_neighbors = 0;
            vertex.forbidden_mask = 0;
            vertex.is_trapped = false;
            vertex.next_in_bucket = nullptr;
            vertex.prev_in_bucket = nullptr;
        }
    }

    // Simple text format: first line: n m (num vertices, num edges)
    Graph Graph::from_edge_list_file(const std::string& path) {
        std::ifstream in(path);
        if (!in) {
            throw std::runtime_error("Could not open graph file: " + path);
        }
        int n, m;
        in >> n >> m;
        Graph g(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            in >> u >> v;
            g.add_edge(u, v);
        }
        g.compute_degrees();
        return g;
    }


}