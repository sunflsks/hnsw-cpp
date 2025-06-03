#include <vector>
#include "vector.hpp"
#pragma once

double uniform_distribution(void);
std::vector<std::vector<double>> ingest_data(uint64_t count = 1, std::string path = "100M.u8bin");  // count is in millions