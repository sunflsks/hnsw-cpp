#include <iostream>
#include <stdexcept>
#include "utils.hpp"
#include <HNSW.hpp>

int main(int argc, char** argv) {
    if (argc < 2) {
        throw std::invalid_argument("Missing args");
    }

    auto path = argv[1];

    auto vecs = ingest_data(path);

    HNSW hnsw;
    std::cout << "Inserting..." << std::endl;
    hnsw.insert(vecs);
    std::cout << "Done inserting. Stats:" << std::endl;
    for (int i = 0; i <= MAXIMUM_LEVEL; i++) {
        std::cout << "Number of vectors at level " << i << " - " << hnsw.count_per_level(i) << std::endl;
    }

    std::cout << vecs.size() << std::endl;
}