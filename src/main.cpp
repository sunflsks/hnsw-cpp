#include <iostream>
#include <stdexcept>
#include "utils.hpp"
#include <HNSW.hpp>
#include <indicators.hpp>

int main(int argc, char** argv) {
    if (argc < 2) {
        throw std::invalid_argument("Missing args");
    }

    auto path = argv[1];

    auto count = 1;
    auto vecs = ingest_data(path, count);

    HNSW hnsw;
    std::cout << "Inserting..." << std::endl;
    hnsw.insert(vecs);
    std::cout << "Done inserting. Stats:" << std::endl;
    for (int i = 0; i <= MAXIMUM_LEVEL; i++) {
        std::cout << "Number of vectors at level " << i << " - " << hnsw.count_per_level(i) << std::endl;
    }

    indicators::ProgressBar search_bar {
        indicators::option::BarWidth{50},
        indicators::option::Start{"["},
        indicators::option::Fill{"="},
        indicators::option::Lead{">"},
        indicators::option::Remainder{"-"},
        indicators::option::End{" ]"},
        indicators::option::ForegroundColor{indicators::Color::blue},
        indicators::option::ShowPercentage{true},
        indicators::option::ShowElapsedTime{true},
        indicators::option::ShowRemainingTime{true},
        indicators::option::PrefixText{"Search Progress"},
        indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
    };

    auto correct = 0;
    auto query_count = (count * COUNT_MULTIPLIER) / 10;
    for (auto i = 0; i < query_count; i++) {
        auto vec = vecs[i];
        auto guess = hnsw.search(vec);
        if (vec == guess) {
            correct++;
        }

        if (i % 50 == 0) search_bar.set_progress((static_cast<double>(i) / query_count) * 100);

    }

    std::cout << "FINAL ACCURACY: " << (static_cast<double>(correct) / query_count) << std::endl;

    for (auto i : vecs) {
        delete i;
    }
}
