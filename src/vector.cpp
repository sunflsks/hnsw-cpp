#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include "utils.hpp"

#define M_MAX 20

// index by world (0, 1, etc). gives all neighbors.

HNSWVector::HNSWVector(const Eigen::VectorXd& vec) {
    this->vec = vec;
}

double HNSWVector::similarity(HNSWVector& vec2, std::string measure = "cosine") {
    if (measure == "cosine") {
        return (vec.dot(vec2.vec)) / (vec.norm() * vec2.vec.norm());
    }

    else if (measure == "l2") {
        return (1 / (1 + (vec - vec2.vec).norm()));
    }

    return 0.;
}

void HNSWVector::insert(HNSWVector& vec_to_insert) { // SHOULD ONLY BE CALLED ON HEAD NODE! otherwise UB.
    static const auto mL = 1 / std::log(M_MAX); // avoid recomputation

    auto level = -(std::log(uniform_distribution()) * mL);
    std::cout << "WILL BE PUT AT LEVEL " << level << std::endl;
}

const Eigen::VectorXd& HNSWVector::getVec() const {
    return vec;
}

bool operator==(const HNSWVector& lhs, const HNSWVector& rhs) {
    return lhs.getVec().isApprox(rhs.getVec());
}

std::size_t HNSWVector::hash() const {
    std::size_t seed = 0;
    for (int i = 0; i < vec.size(); ++i) {
        seed ^= std::hash<double>{}(vec[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    return seed;
}