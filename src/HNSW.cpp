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
        auto maxheap = a_star_search(vec_to_insert, entry, i);
        auto mult = (i == 0) ? 2 : 1;
        auto new_conns = heuristic_1(vec_to_insert, maxheap, M_MAX * mult, i);

        for (auto vec : new_conns) {
            // because of the resizing, we might
            // have a bunch of nullptrs at the end. skip attempting to insert them.
            if (vec == nullptr) break;
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

MaxVectorHeap HNSW::a_star_search(HNSWVector* query, HNSWVector* entry, int level) {
    MinVectorHeap minheap((ClosestFirstVectorComparator(query))); // A* heap
    MaxVectorHeap maxheap((FarthestFirstVectorComparator(query))); // heap of best candidates

    for (auto vec : entry->neighbors(level)) {
        // neighbors(i) returns all neighbors of entry point
        // this includes the entry point itself
        minheap.push(vec);
    }

    // finding best neighbors.
    while(!minheap.empty()) {
        auto best = minheap.pop();

        for (HNSWVector* neighbor : best->neighbors(level)) {
            if (!maxheap.get_set().count(neighbor)) { // if maxheap does not have neighbor.
                auto dist = query->distance(*neighbor);

                // if maxheap is not full, we add like normal
                if (maxheap.size() < EFCONSTRUCTION) {
                    maxheap.push(neighbor);
                } else {
                    // MAXHEAP IS FULL! we get the (possible) worst value from the top
                    auto possible_worst = maxheap.top();

                    // if the worst value out of all the candidates is WORSE than the possible
                    // candidate we are checking, we throw it away and add our new (slightly better)
                    // candidate.
                    if (dist < query->distance(*possible_worst)) {
                        maxheap.pop();
                        maxheap.push(neighbor);
                    }
                }
            }
        }
        // if we are done with A* search AND/OR the best possible node we can pop is WORSE than
        // the worst possible candidate we already have, we exit, as there is no possible gain.
        if (minheap.empty() || query->distance(*minheap.top()) > query->distance(*maxheap.top())) {
            return maxheap;
        }
    }

    return maxheap;
}

void HNSW::insert(std::vector<HNSWVector*>& vectors) {
    for (auto vec : vectors) {
        insert(vec);
    }
}

HNSWVector* HNSW::greedy_search(HNSWVector* query, HNSWVector* ep, int level) {
    while(true) {
        auto closest_neighbor = ep->closest_neighbors(*query, level, 1).front();
        if (closest_neighbor == ep) {
            return ep;
        }

        ep = closest_neighbor;
    }
}

HNSWVector* HNSW::search(HNSWVector* query) { // nearest neighbor
    auto cur = entry_point;
    for (auto l = max_level; l >= 0; l--) {
        auto greedy_result = greedy_search(query, cur, l);
        cur = greedy_result;
    }

    auto closest_possible_vecs = a_star_search(query, cur, 0).get_set();
    auto best_vec = *closest_possible_vecs.begin();
    for (auto vec : closest_possible_vecs) {
        if (query->distance(*vec) < query->distance(*best_vec)) {
                best_vec = vec;
        }
    }

    return best_vec;
}

// level is not used in heuristic 1 -- we only need it to maintain API compat w/ heuristic 2
std::vector<HNSWVector*> HNSW::heuristic_1(HNSWVector* query, MaxVectorHeap& candidates, int count, int level) {
    // get best M nodes. if on layer 0, get best 2*M nodes. Heuristic 1 (from paper)
    std::vector<HNSWVector*> new_conns;

    while (true) {
        if (candidates.size() > 0) {
            new_conns.push_back(candidates.top());
            candidates.pop();
        } else {
            break;
        }
    }

    std::reverse(new_conns.begin(), new_conns.end());
    new_conns.resize(count);

    return new_conns;
}