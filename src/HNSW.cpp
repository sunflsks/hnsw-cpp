#include <HNSWVector.hpp>
#include <HNSW.hpp>
#include <utils.hpp>
#include <algorithm>
#include <iostream>
#include <queue>


void HNSW::insert(HNSWVector* vec_to_insert) { // SHOULD ONLY BE CALLED ON HEAD NODE! otherwise UB.
    static const auto mL = 1 / std::log(M_MAX); // avoid recomputation
    auto level = std::min(int(-(std::log(uniform_distribution()) * mL)), MAXIMUM_LEVEL);

    vec_to_insert->max_level = level;

    // ALGORITHM - we recurse down from the top.

    // if nothing, then we return.
    if (!this->entry_point) {
        // no entry point defined; we make it ourselves.
        this->entry_point = vec_to_insert;
        vec_to_insert->neighbors_for_world = MultiLevelNeighborMap(level + 1);
        this->max_level = level;
        this->entry_point->is_valid = true;
        return;
    }

    // greedy search - find all the closest nodes at each level UP TO the level the node will be
    // inserted at. push onto list and drop down.
    auto entry = this->entry_point;
    for (int i = this->max_level; i > level; i--) {
        auto closest_neighbor = entry->closest_neighbors(*vec_to_insert, i, 1).front();
        entry = closest_neighbor;
    }

    /*
     * we now conduct A* search on each level SEPARATELY!!!
     * the neighbors each node has (at a certain level) are usually not
     * the same between levels
    */
    for (int i = std::min(max_level, level); i >= 0; i--) {
        Heap<HNSWVector*, VectorComparator> minheap(VectorComparator(*vec_to_insert, 0)); // A* heap
        Heap<HNSWVector*, VectorComparator> maxheap(VectorComparator(*vec_to_insert, 1)); // heap of best candidates

        for (auto vec : entry->neighbors(i)) {
            // neighbors(i) returns all neighbors of entry point
            // this includes the entry point itself
            minheap.push(vec);
        }

        // finding best neighbors.
        while(!minheap.empty()) {
            auto best = minheap.pop();

            for (HNSWVector* neighbor : best->neighbors(i)) {
                if (!maxheap.get_set().count(neighbor)) { // if maxheap does not have neighbor.
                    auto dist = vec_to_insert->similarity(*neighbor);

                    // if maxheap is not full, we add like normal
                    if (maxheap.size() < EFCONSTRUCTION) {
                        maxheap.push(neighbor);
                    } else {
                        // MAXHEAP IS FULL! we get the (possible) worst value from the top
                        auto possible_worst = maxheap.top();

                        // if the worst value out of all the candidates is WORSE than the possible
                        // candidate we are checking, we throw it away and add our new (slightly better)
                        // candidate.
                        if (dist < vec_to_insert->similarity(*possible_worst)) {
                            maxheap.pop();
                            maxheap.push(neighbor);
                        }
                    }
                }
            }

            // if we are done with A* search AND/OR the best possible node we can pop is WORSE than
            // the worst possible candidate we already have, we exit, as there is no possible gain.
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
        this->entry_point = vec_to_insert;
    }

    // vector has been properly inserted and is valid in the context of the database.
    this->_count_per_level[level]++;
    vec_to_insert->is_valid = true;
    this->all_vectors.push_back(vec_to_insert);
}

void HNSW::insert(std::vector<HNSWVector*>& vectors) {
    for (auto vec : vectors) {
        insert(vec);
    }
}

HNSWVector* HNSW::search(HNSWVector* query) { // nearest neighbor
    auto cur = entry_point;
    for (auto l = max_level; l >= 0; l--) {
        auto nearest_neighbor = cur->closest_neighbors(*query, l, 1).front();
        cur = nearest_neighbor;
    }

    return cur;
}