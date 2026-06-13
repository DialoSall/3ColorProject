/*
test_graphs.cpp

Author: Dialo Sall

Test runner for different graphs
Simply prints results, does not use checks or assertions yet

Has a success graph, fail graph, and random graph

*/
#include <iostream>
#include <cassert>
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
        assert(ok);
        assert(solver.verify_coloring());
        std::cout << "  Result: " << (ok ? "SUCCESS" : "FAILURE") << "\n";
        
    }

    {
        std::cout << "Test graph 2:\n";
        Graph g = build_small_test_graph_2();
        ThreeColorSolver solver(g);
        bool ok = solver.run_greedy();
        assert(!ok);
        std::cout << "  Result: " << (ok ? "SUCCESS" : "FAILURE") << "\n";
    }

    // NEW: random graph test block
    {
        std::cout << "Random graph test:\n";
        auto rng = make_rng();
        Graph g = generate_random_graph(10, 0.3, rng);
        ThreeColorSolver solver(g);
        bool ok = solver.run_greedy();
        if(ok) {
            assert(solver.verify_coloring());
        }
        std::cout << "  Result: " << (ok ? "SUCCESS" : "FAILURE") << "\n";
    }

    return 0;
}
