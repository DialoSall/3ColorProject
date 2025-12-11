/* 
main.cpp
Author: Dialo Sall
*/

#include <iostream>
#include "graph.hpp"
#include "algorithm.hpp"
#include "utilities.hpp"

using namespace threecolor;

int main(int argc, char** argv) {
    try {
        Graph g;

        if (argc > 1) {
            // Load from file if path provided
            g = Graph::from_edge_list_file(argv[1]);
        } else {
            // Otherwise generate a small random graph
            auto rng = make_rng();
            g = generate_random_graph(10, 0.3, rng);
            g.compute_degrees();
        }

        ThreeColorSolver solver(g);
        bool ok = solver.run_greedy();

        std::cout << "Greedy 3-color result: " << (ok ? "Success" : "Failure") << "\n";

        if (ok && solver.verify_coloring()) {
            std::cout << "Coloring verified.\n";
        } else if (ok) {
            std::cout << "Coloring appears invalid.\n";
        }

        // Optional: export to DOT
        // export_to_dot(g, "output.dot");

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
