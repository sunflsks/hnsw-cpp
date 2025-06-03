#include <iostream>
#include <stdexcept>
#include "utils.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        throw std::invalid_argument("Missing args");
    }

    auto path = argv[1];

    auto vecs = ingest_data(path);
    std::cout << vecs.size() << std::endl;
}