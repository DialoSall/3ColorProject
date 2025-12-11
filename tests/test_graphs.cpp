#include <iostream>
#include "graph.hpp"
#include "algorithm.hpp"
#include "utilities.hpp"

using namespace threecolor;

int main() {
    {
        std::cout << "Test graph 1:\n";
        Graph g = build_small_test_graph_1();
        ThreeColorSolver solver(g);
        bool ok = solver.run_greedy();
        std::cout << "  Result: " << (ok ? "SUCCESS" : "FAILURE") << "\n";
    }

    {
        std::cout << "Test graph 2:\n";
        Graph g = build_small_test_graph_2();
        ThreeColorSolver solver(g);
        bool ok = solver.run_greedy();
        std::cout << "  Result: " << (ok ? "SUCCESS" : "FAILURE") << "\n";
    }

    return 0;
}
