/* 
main.cpp

Author: Dialo Sall

If a file path is provided, main.cpp loads a graph from the file.
Otherwise, a random graph is generated

ThreeColorSolver is executed on the graph, and a success/failure is printed
Verifies coloring after success
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
        if (ok) {
            export_to_dot(g, "colored_success.dot");
            std::cout << "Graph exported to colored_success.dot\n";
        } else {
            export_to_dot(g, "colored_failure.dot");
            std::cout << "Graph exported to colored_failure.dot\n";
        }

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
