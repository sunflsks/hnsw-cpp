#pragma once

#include <vector>
#include <optional>
#include <consts.hpp>
#include <utils.hpp>
#include <HNSWVector.hpp>


// main HNSW class -- doesn't really have TOO much besides the insertion logic (which is everything lol)
class HNSW {
    private:
        std::vector<HNSWVector*> all_vectors; // we mostly use this just to count all the vectors at the end, but could come in handy later (testing, etc)

        int max_level = 0; // maximum level of the HNSW graph. starts at 0
        int _count_per_level[MAXIMUM_LEVEL + 1] = {0};

        // top-level vector; this will always be the entry point.
        // multiple vectors can share this entryPoint's level, but this will be the start always.
        HNSWVector* entry_point = nullptr;

    public:
        void insert(HNSWVector* vector); // insert one vec
        void insert(std::vector<HNSWVector*>& vectors); // wrapper for previous
        int count_per_level(int level) { return this->_count_per_level[level]; }
        HNSWVector* search(HNSWVector* query); // nearest neighbor
        HNSWVector* greedy_search(HNSWVector* query, HNSWVector* entry, int level);
        Heap<HNSWVector*, FarthestFirstVectorComparator> a_star_search(HNSWVector* query, HNSWVector* entry, int level);
};