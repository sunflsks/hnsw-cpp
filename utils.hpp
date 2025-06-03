#include <vector>
#include "vector.hpp"
#pragma once

double uniform_distribution(void);
std::vector<HNSWVector> ingest_data(uint64_t count = 1, std::string path = "100M.u8bin");  // count is in millions