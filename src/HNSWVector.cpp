#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <utils.hpp>

HNSWVector::HNSWVector(const Eigen::VectorXd& vec) {
    this->vec = vec;
}

HNSWVector::HNSWVector() {}

double HNSWVector::similarity(const HNSWVector& vec2, std::string measure) const {
    if (measure == "cosine") {
        return (vec.dot(vec2.vec)) / (vec.norm() * vec2.vec.norm());
    }

    else if (measure == "l2") {
        return (1 / (1 + (vec - vec2.vec).norm()));
    }

    return 0.;
}

double HNSWVector::distance(const HNSWVector& vec2, std::string measure) const {
    if (measure == "cosine") {
        return 1 - similarity(vec2, "cosine");
    }

    else if (measure == "l2") {
        return (vec - vec2.vec).norm();
    }

    return 0.;
}

std::vector<HNSWVector*> HNSWVector::neighbors(int world) {
    std::vector<HNSWVector*> neighbor_vec;
    for (auto vec: this->neighbors_for_world[world]) neighbor_vec.push_back(vec);
    neighbor_vec.push_back(this);

    return neighbor_vec;
}

std::vector<HNSWVector*> HNSWVector::closest_neighbors(HNSWVector& query, int level, int k) {
    std::vector<HNSWVector*> neighbor_vec;
    for (auto vec: this->neighbors_for_world[level]) neighbor_vec.push_back(vec);
    neighbor_vec.push_back(this);

    std::sort(neighbor_vec.begin(), neighbor_vec.end(), [query](auto a, auto b) {
            return query.distance(*a) < query.distance(*b);
    });

    neighbor_vec.resize(k);
    return neighbor_vec;
}

std::size_t HNSWVector::hash() const {
    std::size_t seed = 0;
    for (int i = 0; i < vec.size(); ++i) {
        seed ^= std::hash<double>{}(vec[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    return seed;
}

void HNSWVector::connect(int level, HNSWVector& vec2) {
    this->neighbors_for_world[level].insert(&vec2);
    vec2.neighbors_for_world[level].insert(this);
}

bool HNSWVector::operator==(const HNSWVector& rhs) {
    return this->vec.isApprox(rhs.vec);
}