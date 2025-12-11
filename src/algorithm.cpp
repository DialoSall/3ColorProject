#include "algorithm.hpp"

namespace threecolor {

ThreeColorSolver::ThreeColorSolver(Graph& g)
    : graph_(g),
      machine_(),
      trapped_head_(nullptr) {}

bool ThreeColorSolver::run_greedy() {
    initialize_state();

    while (true) {
        if (machine_.empty()) {
            // All uncolored vertices removed → success
            return true;
        }

        // 1. Trapped vertex first
        Vertex* v = pop_trapped();
        if (v) {
            // ensure it's also removed from sorting machine
            machine_.remove(v);
        } else {
            // 2. Else pick from sorting machine
            v = machine_.pick_next_vertex();
            if (!v) {
                // Should mean all vertices colored
                return true;
            }
            machine_.remove(v);
        }

        // Determine color
        Color c = trapped_color(v);
        if (c == UNCOLORED) {
            c = choose_color_with_priority(v);
        }

        if (c == UNCOLORED) {
            // No available color
            return false; // Greedy failure; with backtracking you'd branch here
        }

        v->color = c;

        // Update neighbors; if we hit contradiction, fail
        if (!update_neighbors(v, c)) {
            return false;
        }
    }
}

void ThreeColorSolver::initialize_state() {
    graph_.reset_coloring_state();
    trapped_head_ = nullptr;
    machine_.initialize(graph_);
}

void ThreeColorSolver::add_trapped(Vertex* v) {
    // Simple push-front onto singly linked list using next_in_bucket as next ptr
    if (v->is_trapped) return;
    v->is_trapped = true;
    v->next_in_bucket = trapped_head_;
    trapped_head_ = v;
}

void ThreeColorSolver::remove_trapped(Vertex* v) {
    if (!v->is_trapped) return;
    // linear removal from trapped list; OK because trapped list should be small
    Vertex* prev = nullptr;
    Vertex* cur = trapped_head_;
    while (cur) {
        if (cur == v) {
            if (prev) prev->next_in_bucket = cur->next_in_bucket;
            else trapped_head_ = cur->next_in_bucket;
            break;
        }
        prev = cur;
        cur = cur->next_in_bucket;
    }
    v->is_trapped = false;
    v->next_in_bucket = nullptr;
}

Vertex* ThreeColorSolver::pop_trapped() {
    Vertex* v = trapped_head_;
    while (v && v->color != UNCOLORED) {
        // skip any that might have been colored later
        trapped_head_ = v->next_in_bucket;
        v->is_trapped = false;
        v->next_in_bucket = nullptr;
        v = trapped_head_;
    }
    if (!v) return nullptr;

    trapped_head_ = v->next_in_bucket;
    v->is_trapped = false;
    v->next_in_bucket = nullptr;
    return v;
}

int ThreeColorSolver::available_color_count(const Vertex* v) const {
    uint8_t avail = static_cast<uint8_t>(~v->forbidden_mask) & 0b111;
    int count = 0;
    while (avail) {
        count += (avail & 1);
        avail >>= 1;
    }
    return count;
}

Color ThreeColorSolver::trapped_color(const Vertex* v) const {
    uint8_t avail = static_cast<uint8_t>(~v->forbidden_mask) & 0b111;
    int count = 0;
    int last_color = -1;
    for (int c = 0; c < 3; ++c) {
        if ((avail >> c) & 1) {
            ++count;
            last_color = c;
        }
    }
    if (count == 1) {
        return static_cast<Color>(last_color);
    }
    return UNCOLORED;
}

Color ThreeColorSolver::choose_color_with_priority(const Vertex* v) const {
    // Color priority: RED, then BLUE, then YELLOW
    uint8_t forb = v->forbidden_mask;
    if ((forb & (1 << RED)) == 0)    return RED;
    if ((forb & (1 << BLUE)) == 0)   return BLUE;
    if ((forb & (1 << YELLOW)) == 0) return YELLOW;
    return UNCOLORED;
}

bool ThreeColorSolver::update_neighbors(Vertex* v, Color c) {
    for (Neighbor* p = v->neighbors_head; p != nullptr; p = p->next) {
        Vertex* u = p->v;
        if (u->color != UNCOLORED) continue;

        // Update forbidden mask
        uint8_t old_mask = u->forbidden_mask;
        u->forbidden_mask = static_cast<uint8_t>(u->forbidden_mask | (1 << c));

        // If mask didn't change, no new restriction
        if (u->forbidden_mask != old_mask) {
            int avail_count = available_color_count(u);
            if (avail_count == 0) {
                // No color available: contradiction
                return false;
            } else if (avail_count == 1) {
                add_trapped(u);
            } else {
                // Not trapped anymore if previously trapped
                if (u->is_trapped) {
                    remove_trapped(u);
                }
            }
        }

        // Bump colored_neighbors and reposition in sorting machine
        machine_.bump_colored_neighbors(u);
    }
    return true;
}

bool ThreeColorSolver::verify_coloring() const {
    for (int i = 0; i < graph_.num_vertices(); ++i) {
        const Vertex& v = graph_.vertex(i);
        if (v.color == UNCOLORED) return false;
        for (Neighbor* p = v.neighbors_head; p != nullptr; p = p->next) {
            if (p->v->color == v.color) return false;
        }
    }
    return true;
}

} // namespace threecolor
