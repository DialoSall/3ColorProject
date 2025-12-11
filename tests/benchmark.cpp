#include <iostream>
#include <chrono>
#include "graph.hpp"
#include "algorithm.hpp"
#include "utilities.hpp"

using namespace threecolor;

int main() {
    auto rng = make_rng();

    for (int n : {20, 40, 60, 80, 100}) {
        for (double p : {0.1, 0.2, 0.3, 0.4}) {
            int trials = 10;
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
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            std::cout << "n=" << n << ", p=" << p
                      << " -> successes=" << successes << "/" << trials
                      << ", time=" << ms << " ms\n";
        }
    }

    return 0;
}
