#include <vector>
#include <HNSWVector.hpp>
#pragma once

double uniform_distribution(void); // generates random floats between 0 -> 1
std::vector<HNSWVector> ingest_data(uint64_t count = 1, std::string path = "100M.u8bin");  // count is in millions