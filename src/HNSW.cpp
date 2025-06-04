#include <HNSWVector.hpp>
#include <HNSW.hpp>
#include <utils.hpp>
#include <algorithm>
#include <iostream>
#include <queue>

#define M_MAX 10
#define EFCONSTRUCTION 30

void HNSW::insert(HNSWVector* vec_to_insert) { // SHOULD ONLY BE CALLED ON HEAD NODE! otherwise UB.
    static const auto mL = 1 / std::log(M_MAX); // avoid recomputation
    auto level = std::min(int(-(std::log(uniform_distribution()) * mL)), 5);

    vec_to_insert->max_level = level;

    // ALGORITHM - we recurse down from the top.

    // if nothing, then we return.
    if (!this->entryPoint) {
        // no entry point defined; we make it ourselves.
        this->entryPoint = vec_to_insert;
        vec_to_insert->neighbors_for_world = MultiLevelNeighborMap(level + 1);
        this->max_level = level;
        this->entryPoint->is_valid = true;
        return;
    }

    // greedy search - find all the closest nodes at each level UP TO the level the node will be inserted at. push onto list and drop down.
    auto entry = this->entryPoint;
    for (int i = this->max_level; i > level; i--) {
        auto closest_neighbor = entry->closest_neighbors(*vec_to_insert, i, 1).front();
        entry = closest_neighbor;
    }

    // greedy_search_list now has all the vectors we can link to. we now link them all
    for (int i = std::min(max_level, level); i >= 0; i--) {
        Heap<HNSWVector*, VectorComparator> minheap(VectorComparator(*vec_to_insert, 0));
        Heap<HNSWVector*, VectorComparator> maxheap(VectorComparator(*vec_to_insert, 1));

        for (auto vec : entry->neighbors(i)) {
            minheap.push(vec); // neighbors has all neighbors of entry point (including entry point itself)
        }

        // finding best neighbors.
        while(!minheap.empty()) {
            auto best = minheap.pop();

            for (HNSWVector* neighbor : best->neighbors(i)) {
                if (!maxheap.get_set().count(neighbor)) {
                    auto dist = vec_to_insert->similarity(*neighbor);

                    if (maxheap.size() < EFCONSTRUCTION) {
                        maxheap.push(neighbor);
                    } else {
                        auto possible_worst = maxheap.top();

                        if (dist < vec_to_insert->similarity(*possible_worst)) {
                            maxheap.pop();
                            maxheap.push(neighbor);
                        }
                    }
                }
            }

            if (minheap.empty() || vec_to_insert->similarity(*minheap.top()) > vec_to_insert->similarity(*maxheap.top())) {
                break;
            }
        }

        // get best M nodes. if on layer 0, get best 2*M nodes. Heuristic 1 (from paper)
        auto mult = (i == 0 ? 2 : 1);
        std::vector<HNSWVector*> new_conns;
        for (auto j = 0; j < (mult * M_MAX); j++) {
            if (maxheap.size() > 0) {
                new_conns.push_back(maxheap.top());
                maxheap.pop();
            }
        }

        for (auto vec : new_conns) {
            vec_to_insert->connect(i, *vec);
        }
    }

    // if new level > max level, we promote entry point
    if (level > this->max_level) {
        this->max_level = level;
        this->entryPoint = vec_to_insert;
    }

    vec_to_insert->is_valid = true;
    this->all_vectors.push_back(vec_to_insert);
}