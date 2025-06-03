#pragma once

#include <vector>
#include <HNSWVector.hpp>

// main HNSW class -- doesn't really have TOO much besides the insertion logic (which is everything lol)
class HNSW {
    private:
        std::vector<HNSWVector> all_vectors;
        int max_level; // maximum level of the HNSW graph

        // top-level vector; this will always be the entry point.
        // multiple vectors can share this entryPoint's level, but this will be the start always.
        HNSWVector entryPoint;

    public:
        void insert(HNSWVector& vector);
        void insert(std::vector<HNSWVector>& vectors);
};