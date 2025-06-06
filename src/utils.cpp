#include <random>
#include <vector>
#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include <stdexcept>
#include <utils.hpp>
#include <consts.hpp>
#include <HNSWVector.hpp>

double uniform_distribution(void) {
    static auto gen = std::mt19937(RNG_SEED);
    static auto distr = std::uniform_real_distribution<double>(1e-8, 1); // avoid divide-by-zero and log(0)

    return distr(gen);
}

std::vector<HNSWVector*> ingest_data(std::string path, uint64_t count) { // count is in millions
    std::ifstream binfile(path, std::ios::binary);
    std::vector<HNSWVector*> vector_buffer;
    vector_buffer.reserve(count * COUNT_MULTIPLIER);

    if (!binfile.is_open()) {
        return vector_buffer; // just return empty vector.
    }

    uint32_t num_dimensions;
    uint32_t num_points;

    binfile.read(reinterpret_cast<char*>(&num_points), sizeof(uint32_t));
    binfile.read(reinterpret_cast<char*>(&num_dimensions), sizeof(uint32_t));

    std::cout << "Ingesting " << count * COUNT_MULTIPLIER << "/" << num_points << " points." << std::endl;

    for (uint64_t i = 0; i < (count * COUNT_MULTIPLIER); i++) {
        std::vector<uint8_t> vector(num_dimensions);
        if (!binfile.read(reinterpret_cast<char*>(vector.data()), vector.size())) {
            throw std::runtime_error("Error in reading file");
        }

        std::vector<double> new_vec(num_dimensions);
        std::transform(vector.begin(), vector.end(), new_vec.begin(), [](uint8_t x){ return (double)x; });
        auto eigen_vec = Eigen::VectorXd::Map(new_vec.data(), new_vec.size());
        vector_buffer.push_back(new HNSWVector(eigen_vec));
    }

    std::cout << "Done ingesting -- return buffer is of size " << vector_buffer.size() << std::endl;

    return vector_buffer;
}
MinVectorHeap max_to_min_heap(MaxVectorHeap& maxheap, HNSWVector* query) {
    MinVectorHeap minheap((ClosestFirstVectorComparator(query)));
    for (auto element : maxheap.get_set()) {
        minheap.push(element);
    }

    return minheap;
}

void free_data(std::vector<HNSWVector*> vectors) {
    for (auto vec : vectors) {
        delete vec;
    }
}