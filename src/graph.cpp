/*
graph.cpp

Author: Dialo Sall

Defines a graph, vertex, and the relationship between vertices.
Implements functions defined in graph.hpp

*/
#include "graph.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace threecolor {
    
    Vertex::Vertex() // initializes a vertex with no id, no color, no neighbors
        : id(-1), degree(0), color(UNCOLORED), colored_neighbors(0), forbidden_mask(0), 
        is_trapped(false), neighbors_head(nullptr), next_in_bucket(nullptr), prev_in_bucket(nullptr) {}

    Graph::Graph(int n) // initializes graph with n vertices
        : vertices(n)  // reserves space for n vertices
    {
        for (int i = 0; i < n; ++i) {
            vertices[i].id = i; // assign unique id to each vertex
        }
    }

    void Graph::clear_neighbors() {
        for(auto& vertex : vertices) {
            Neighbor* cur = vertex.neighbors_head;
            while(cur != nullptr) {
                Neighbor* next = cur->next;
                delete cur;
                cur = next;
            }
            vertex.neighbors_head = nullptr;
        }
    }

    Graph::Graph(const Graph& other)
        :vertices(other.vertices.size())
    {
        for (int i = 0; i < static_cast<int>(vertices.size()); ++i) {
            vertices[i].id = other.vertices[i].id;
            vertices[i].degree = other.vertices[i].degree;
            vertices[i].color = other.vertices[i].color;
            vertices[i].colored_neighbors = other.vertices[i].colored_neighbors;
            vertices[i].forbidden_mask = other.vertices[i].forbidden_mask;
            vertices[i].is_trapped = other.vertices[i].is_trapped;
            vertices[i].neighbors_head = nullptr;
            vertices[i].next_in_bucket = nullptr;
            vertices[i].prev_in_bucket = nullptr;
        }

        //Rebuild adjacency lists
        for (int u = 0; u < other.num_vertices(); ++u) {
            for (Neighbor* p = other.vertices[u].neighbors_head; p != nullptr; p = p->next) {
                int v = p->v->id;

                // Avoid duplicating undirected edges twice
                if (u < v) {
                    add_edge(u, v);
                }
            }
        }

        compute_degrees();
    }

    Graph& Graph::operator=(const Graph& other) {
        if (this == &other) return *this;

        clear_neighbors();

        Graph temp(other);
        *this = std::move(temp);

        return *this;
    }

    Graph::Graph(Graph&& other) noexcept
        : vertices(std::move(other.vertices))
    {
        other.vertices.clear();
    }

    Graph& Graph::operator=(Graph&& other) noexcept {
        if (this == &other) return *this;

        clear_neighbors();
        vertices = std::move(other.vertices);
        other.vertices.clear();

        return *this;
    }

    void Graph::add_edge(int u, int v) { // creates an edge (connection) between two vertices
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

    void Graph::compute_degrees() { // Computes degree of vertex, stores it in vertex member field
        for (auto& vertex : vertices) {
            int d = 0;
            for (Neighbor* nbr = vertex.neighbors_head; nbr != nullptr; nbr = nbr->next) {
                ++d;
            }
            vertex.degree = d;
        }
    }

    void Graph::reset_coloring_state() { // resets all information regarding 3 color attempt
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