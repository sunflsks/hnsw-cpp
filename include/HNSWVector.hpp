#pragma once

#include <map>
#include <consts.hpp>
#include <Eigen/Dense>
#include <unordered_set>

class HNSWVector;

using VectorSet = std::unordered_set<HNSWVector*>;
using MultiLevelNeighborMap = std::vector<VectorSet>;

class HNSWVector {

    private:
        bool is_valid = 0;
        int max_level = 0; // up to what level does this vector go to? entry point will the at the top.
        Eigen::VectorXd vec; // actual vector that is represented by this class
        MultiLevelNeighborMap neighbors_for_world = std::vector<VectorSet>(MAXIMUM_LEVEL + 1);

        friend class HNSW;
    public:
        HNSWVector(const Eigen::VectorXd& vec);// ctor
        HNSWVector(); // ctor
        bool operator==(const HNSWVector& rhs) {
            return this->vec.isApprox(rhs.vec);
        }

        std::size_t hash() const; // for hash func -- map, etc
        std::vector<HNSWVector*> closest_neighbors(HNSWVector& query, int level, int k = 1);
        std::vector<HNSWVector*> neighbors(int world);
        void connect(int level, HNSWVector& vec2);

        double similarity(const HNSWVector& vec2, std::string measure = "cosine") const;
};

bool operator==(const HNSWVector& lhs, const HNSWVector& rhs);

template <>
struct std::hash<HNSWVector> {
    std::size_t operator()(const HNSWVector& v) const {
        return v.hash();
    }
};

struct VectorComparator {
    const HNSWVector& ref;
    bool min_heap = 0;

    VectorComparator(const HNSWVector& ref, bool min_heap = false)
        : ref(ref), min_heap(min_heap) {}

    bool operator()(const HNSWVector* a, const HNSWVector* b) const {
        if (min_heap) {
            return ref.similarity(*a) < ref.similarity(*b);
        }

        return ref.similarity(*a) > ref.similarity(*b);
    }
};