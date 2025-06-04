#pragma once

#include <vector>
#include <HNSWVector.hpp>

double uniform_distribution(void); // generates random floats between 0 -> 1
std::vector<HNSWVector*> ingest_data(std::string path = "100M.u8bin", uint64_t count = 1);  // count is in millions

/*
 * following is mainly for minheap and maxheap since the STL classes
 * did not really give me what i needed
*/
template <typename T, typename Compare>
class Heap {
    private:
        std::vector<T> container;
        std::unordered_set<T> set;
        Compare compare;

    public:
        Heap(Compare c = Compare()) : compare(c) {}

        void push(T t) {
            container.push_back(t);
            std::push_heap(container.begin(), container.end(), compare);
            set.insert(t);
        }

        T pop() {
            std::pop_heap(container.begin(), container.end(), compare);
            T ret = container.back();
            container.pop_back();
            set.erase(ret);
            return ret;
        }

        const T& top() const {
            return container.front();
        }

        bool empty() const {
            return container.empty();
        }

        std::size_t size() const {
            return container.size();
        }

        const std::unordered_set<T>& get_set() const {
            return set;
        }
};

using MaxVectorHeap = Heap<HNSWVector*, FarthestFirstVectorComparator>;
using MinVectorHeap = Heap<HNSWVector*, ClosestFirstVectorComparator>;