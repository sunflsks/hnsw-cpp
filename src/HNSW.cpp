#include <HNSWVector.hpp>
#include <HNSW.hpp>
#include <utils.hpp>
#include <iostream>

#define M_MAX 20

void HNSW::insert(HNSWVector& vec_to_insert) { // SHOULD ONLY BE CALLED ON HEAD NODE! otherwise UB.
    static const auto mL = 1 / std::log(M_MAX); // avoid recomputation

    auto level = -(std::log(uniform_distribution()) * mL);
    std::cout << "WILL BE PUT AT LEVEL " << level << std::endl;
}