#include <Eigen/Dense>
#include <unordered_set>
#include <map>

#pragma once

class HNSWVector {
    private:
        int max_level; // up to what level does this vector go to? entry point will the at the top.
        Eigen::VectorXd vec; // actual vector that is represented by this class
        std::vector<std::unordered_map<HNSWVector, std::unordered_set<HNSWVector>>> neighbors_for_world;
    public:
        HNSWVector(const Eigen::VectorXd& vec); // ctor
        std::size_t hash() const; // for hash func
        const Eigen::VectorXd& getVec() const;

        void insert(HNSWVector& vec_to_insert); // SHOULD ONLY BE CALLED ON HEAD NODE! otherwise UB.
        double similarity(HNSWVector& vec2, std::string measure);
};

bool operator==(const HNSWVector& lhs, const HNSWVector& rhs);

template <>
struct std::hash<HNSWVector> {
    std::size_t operator()(const HNSWVector& v) const {
        return v.hash();
    }
};