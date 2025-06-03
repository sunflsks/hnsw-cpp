#include <iostream>
#include "utils.hpp"

int main(void) {
    auto vecs = ingest_data();
    std::cout << vecs.size() << std::endl;
}