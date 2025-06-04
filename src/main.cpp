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

    for (auto vec : vecs) {
        hnsw.insert(vec);
        std::cout << "Inserting vector" << std::endl;
    }

    std::cout << vecs.size() << std::endl;
}