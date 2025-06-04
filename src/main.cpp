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

    // try search
    while(true) {
        std::string input;
        std::cout << "Give a number from 0-999 of a vector idx to search:" << std::endl;
        std::cin >> input;
        int input_int = std::stoi(input);
        if (input_int < 0 || input_int > 999) {
            std::cerr << "Invalid input" << std::endl;
            continue;
        }

        std::cout << "Searching for vector " << vecs[input_int] << std::endl;
        hnsw.search(vecs[input_int]);
    }

    std::cout << vecs.size() << std::endl;
}