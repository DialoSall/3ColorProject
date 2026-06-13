/*
benchmark.cpp

Author: Dialo Sall

Runs solve across multiple graph sizes and edge probabilities
Tracks successes and records runtimes

Basically trying to answer how the algorithm behaves as graph size/density increases
*/
#include <iostream>
#include <chrono>
#include "graph.hpp"
#include "algorithm.hpp"
#include "utilities.hpp"

using namespace threecolor;

int main() {
    auto rng = make_rng();

    for (int n : {20, 40, 60, 80, 100}) {
        for (double p : {0.02, 0.05, 0.08, 0.1, 0.15, 0.2}) {
            int trials = 100;
            int successes = 0;

            auto start = std::chrono::steady_clock::now();

            for (int t = 0; t < trials; ++t) {
                Graph g = generate_random_graph(n, p, rng);
                g.compute_degrees();

                ThreeColorSolver solver(g);
                if (solver.run_greedy() && solver.verify_coloring()) {
                    ++successes;
                }
            }

            auto end = std::chrono::steady_clock::now();
            auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();


            std::cout << "n=" << n << ", p=" << p
                      << " -> successes=" << successes << "/" << trials
                      << ", time=" << us << " us\n";
        }
    }

    return 0;
}
