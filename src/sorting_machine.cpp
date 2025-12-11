#include "sorting_machine.hpp"
#include <algorithm>
#include <stdexcept>

namespace threecolor {

    SortingMachine::SortingMachine()
        : graph(nullptr) {}

    void SortingMachine::initialize(Graph& g) {
        graph = &g;

        int n = g.num_vertices();
        degree_values.clear();
        buckets.clear();


        // collect all degree values
        degree_values.reserve(n);
        int max_degree = 0;
        for (int i = 0; i < n; ++i) {
            int d = g.vertex(i).degree;
            degree_values.push_back(d);
            if (d > max_degree) max_degree = d;
        }

        // sort + deduplicate into distinct degrees (descending)
        std::sort(degree_values.begin(), degree_values.end());
        degree_values.erase(std::unique(degree_values.begin(), degree_values.end()), degree_values.end());
        std::reverse(degree_values.begin(), degree_values.end());

        int bucket_count = static_cast<int>(degree_values.size());
        buckets.resize(bucket_count);
        for(int i = 0; i < bucket_count; ++i) {
            buckets[i].degree = degree_values[i];
            buckets[i].head = nullptr;
        }

        // map degree to bucket index
        degree_to_bucket.resize(max_degree + 1, -1);
        for(int i = 0; i < bucket_count; ++i) {
            int d = degree_values[i];
            degree_to_bucket[d] = i;
        }

        // insert all vertices into their buckets
        for(int i = 0; i < n; ++i) {
            Vertex* v = &g.vertex(i);
            insert(v);
        }
    }

    bool SortingMachine::empty() const {
        for (const auto& bucket : buckets) {
            if (bucket.head != nullptr) return false;
        }
        return true;
    }

    int SortingMachine::find_bucket_index_for_degree(int degree) const {
        if (degree < 0 || degree >= static_cast<int>(degree_to_bucket.size())) {
            return -1;
        }
        return degree_to_bucket[degree];
    }

    void SortingMachine::insert(Vertex* v) {
        if (!graph || !v) return;
        if (v->color != UNCOLORED) return; // only uncolored vertices

        int index = find_bucket_index_for_degree(v->degree);
        if (index < 0) {
            throw std::runtime_error("Insert: vertex degree has no bucket");
        }

        Bucket& B = buckets[index];
        Vertex* cur = B.head;
        Vertex* prev = nullptr;

        // Order by colored_neighbors descending, tie-break by id ascending
        while (cur != nullptr && 
               (cur->colored_neighbors > v->colored_neighbors || 
               (cur->colored_neighbors == v->colored_neighbors && cur->id < v->id))) {
            prev = cur;
            cur = cur->next_in_bucket;
        }

        v->prev_in_bucket = prev;
        v->next_in_bucket = cur;

        if(prev) prev->next_in_bucket = v;
        else B.head = v;

        if(cur) cur->prev_in_bucket = v;
    }

    void SortingMachine::remove(Vertex* v) {
        if (!graph || !v) return;
        int index = find_bucket_index_for_degree(v->degree);
        if(index < 0) return;

        Bucket& B = buckets[index];

        if(v->prev_in_bucket) {
            v->prev_in_bucket->next_in_bucket = v->next_in_bucket;
        } else {
            B.head = v->next_in_bucket;
        }
        if(v->next_in_bucket) {
            v->next_in_bucket->prev_in_bucket = v->prev_in_bucket;
        }
        v->next_in_bucket = v->prev_in_bucket = nullptr;
    }

    void SortingMachine::bump_colored_neighbors(Vertex* v) {
        if(!graph || !v) return;
        if(v->color != UNCOLORED) return;

        remove(v);
        v->colored_neighbors++;
        insert(v);
    }
    
    Vertex* SortingMachine::pick_next_vertex() {
        for(auto& bucket : buckets) {
            //Skip over already colored vertices if any somehow remain
            while(bucket.head && bucket.head->color != UNCOLORED) {
                Vertex* colored = bucket.head;
                remove(colored);
            }
            if (bucket.head) return bucket.head;
        }
        return nullptr; // all vertices colored
    }
} // namespace threecolor
