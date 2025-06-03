#include <HNSWVector.hpp>
#include <HNSW.hpp>
#include <utils.hpp>
#include <iostream>
#include <queue>

#define M_MAX 20

void HNSW::insert(HNSWVector& vec_to_insert) { // SHOULD ONLY BE CALLED ON HEAD NODE! otherwise UB.
    static const auto mL = 1 / std::log(M_MAX); // avoid recomputation
    auto level = -(std::log(uniform_distribution()) * mL);

    // ALGORITHM - we recurse down from the top.

    // if nothing, then we return.
    if (!this->entryPoint.is_valid) {
        // no entry point defined; we make it ourselves.
        this->entryPoint = vec_to_insert;
        vec_to_insert.neighbors_for_world = MultiLevelNeighborMap(level + 1);
        vec_to_insert.max_level = level;
        this->max_level = level;
        return;
    }

    // greedy search - find all the closest nodes at each level UP TO the level the node will be inserted at. push onto list.
    std::vector<HNSWVector> greedy_search_list;
    auto cur = this->entryPoint;
    for (int i = this->max_level; i > level; i--) {
        greedy_search_list.push_back(cur);
        auto closest_neighbor = cur.closest_neighbors(level, 1).front();
        cur = closest_neighbor;
    }
}