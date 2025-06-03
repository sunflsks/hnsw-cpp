#pragma once

#include <vector>
#include <HNSWVector.hpp>

double uniform_distribution(void); // generates random floats between 0 -> 1
std::vector<HNSWVector> ingest_data(std::string path = "100M.u8bin", uint64_t count = 1);  // count is in millions