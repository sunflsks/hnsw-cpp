#pragma once

#include <vector>
#include <optional>
#include <consts.hpp>
#include <HNSWVector.hpp>


// main HNSW class -- doesn't really have TOO much besides the insertion logic (which is everything lol)
class HNSW {
    private:
        std::vector<HNSWVector*> all_vectors; // we mostly use this just to count all the vectors at the end, but could come in handy later (testing, etc)

        int max_level = 0; // maximum level of the HNSW graph. starts at 0

        // top-level vector; this will always be the entry point.
        // multiple vectors can share this entryPoint's level, but this will be the start always.
        HNSWVector* entryPoint = nullptr;

    public:
        void insert(HNSWVector* vector); // insert one vec
        void insert(std::vector<HNSWVector*>& vectors); // wrapper for previous
};