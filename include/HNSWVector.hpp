#pragma once

#include <Eigen/Dense>
#include <unordered_set>
#include <map>

class HNSWVector;

using VectorSet = std::unordered_set<HNSWVector>;
using NeighborMap = std::unordered_map<HNSWVector, VectorSet>;
using MultiLevelNeighborMap = std::vector<NeighborMap>;

class HNSWVector {

    private:
        bool is_valid = 0;
        int max_level; // up to what level does this vector go to? entry point will the at the top.
        Eigen::VectorXd vec; // actual vector that is represented by this class
        MultiLevelNeighborMap neighbors_for_world;

        friend class HNSW;
    public:
        HNSWVector(const Eigen::VectorXd& vec); // ctor
        HNSWVector(); // ctor

        std::size_t hash() const; // for hash func -- map, etc
        const Eigen::VectorXd& getVec() const;
        std::vector<HNSWVector> closest_neighbors(int level, int k = 1);

        double similarity(HNSWVector& vec2, std::string measure);
};

bool operator==(const HNSWVector& lhs, const HNSWVector& rhs);

template <>
struct std::hash<HNSWVector> {
    std::size_t operator()(const HNSWVector& v) const {
        return v.hash();
    }
};