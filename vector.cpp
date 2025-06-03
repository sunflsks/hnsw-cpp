#include <Eigen/Dense>
#include <iostream>
#include <cmath>
#include "utils.hpp"

#define M_MAX 20

// index by world (0, 1, etc). gives all neighbors.

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