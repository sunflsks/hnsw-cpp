#include <Eigen/Dense>
#include <unordered_set>
#include <map>

#pragma once

class HNSWVector {
    int max_level; // up to what level does this vector go to? entry point will the at the top.
    Eigen::VectorXd vec; // actual vector that is represented by this class
    std::vector<std::unordered_map<HNSWVector, std::unordered_set<HNSWVector>>> neighbors_for_world;
    double similarity(HNSWVector& vec2, std::string measure);
    void insert(HNSWVector& vec_to_insert); // SHOULD ONLY BE CALLED ON HEAD NODE! otherwise UB.
};