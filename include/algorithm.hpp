#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "graph.hpp"
#include "sorting_machine.hpp"

namespace threecolor {

    class ThreeColorSolver {
    public:
        explicit ThreeColorSolver(Graph& g);

        // Run the algorithm
        // Returns true if a 3-coloring was found, false if a contradiction was reached
        bool run_greedy();

        // verify coloring is correct
        bool verify_coloring() const;

    private:
        Graph& graph_;
        SortingMachine machine_;

        Vertex* trapped_head_; // linked list of trapped vertices

        void initialize_state();
        void add_trapped(Vertex* v);
        void remove_trapped(Vertex* v);
        Vertex* pop_trapped();

        //Update neighbor state after coloring v with color c
        bool update_neighbors(Vertex* v, Color c);

        // Compute number of available colors (popcount of ~forbidden_mask & 0b111)
        int available_color_count(const Vertex* v) const;

        // If vertex is trapped, return the only available color; else UNCOLORED
        Color trapped_color(const Vertex* v) const;

        // Priority-based color choice: RED, then BLUE, then YELLOW if available
        Color choose_color_with_priority(const Vertex* v) const;
    };
} // namespace threecolor

#endif // ALGORITHM_HPP