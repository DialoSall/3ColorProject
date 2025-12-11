/*
utilities.hpp
Author: Dialo Sall
Some utility function headers for graph
*/
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "graph.hpp"
#include <string>
#include <random>

namespace threecolor {

    //random graph generator
    Graph generate_random_graph(int n, double p, std::mt19937& rng);

    //Export graph and coloring to DOT file for visualization
    void export_graph_to_dot(const Graph& g, const std::string& path);

    //Helper to get random seed
    std::mt19937 make_rng();
    
    Graph build_small_test_graph_1();
    Graph build_small_test_graph_2();
} // namespace threecolor

#endif // UTILITIES_HPP

