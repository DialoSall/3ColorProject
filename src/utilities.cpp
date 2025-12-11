#include "utilities.hpp"
#include <random>
#include <fstream>

namespace threecolor {

Graph generate_random_graph(int n, double p, std::mt19937& rng) {
    std::bernoulli_distribution edge_dist(p);
    Graph g(n);
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (edge_dist(rng)) {
                g.add_edge(u, v);
            }
        }
    }
    g.compute_degrees();
    return g;
}

void export_to_dot(const Graph& g, const std::string& path) {
    std::ofstream out(path);
    out << "graph G {\n";
    for (int i = 0; i < g.num_vertices(); ++i) {
        const Vertex& v = g.vertex(i);
        const char* color_str = "white";
        switch (v.color) {
            case RED:    color_str = "red"; break;
            case BLUE:   color_str = "blue"; break;
            case YELLOW: color_str = "yellow"; break;
            case UNCOLORED: color_str = "gray"; break;
        }
        out << "  " << v.id << " [style=filled, fillcolor=" << color_str << "];\n";
    }
    for (int i = 0; i < g.num_vertices(); ++i) {
        const Vertex& v = g.vertex(i);
        for (Neighbor* p = v.neighbors_head; p != nullptr; p = p->next) {
            if (p->v->id > v.id) {
                out << "  " << v.id << " -- " << p->v->id << ";\n";
            }
        }
    }
    out << "}\n";
}

std::mt19937 make_rng() {
    std::random_device rd;
    return std::mt19937(rd());
}


Graph build_small_test_graph_1() {
    // 4 vertices: 0, 1, 2, 3
    Graph g(4);

    // Triangle between 0,1,2
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 0);

    // Vertex 3 connected to 1 and 2 (can become trapped)
    g.add_edge(1, 3);
    g.add_edge(2, 3);

    g.compute_degrees();
    return g;
}


Graph build_small_test_graph_2() {
    // 4 vertices: 0, 1, 2, 3
    Graph g(4);

    // Complete graph K4
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(0, 3);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);

    g.compute_degrees();
    return g;
}


} // namespace threecolor
