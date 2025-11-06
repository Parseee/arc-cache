#include <cstdlib>
#include <iostream>
#include <vector>

#include "Belady/src/belady.hpp"

int main() {
    size_t cache_size, num_calls;
    std::cin >> cache_size >> num_calls;

    caches::BeladyCache<int> cache(cache_size);

    std::vector<int> queries(num_calls);
    for (auto &q : queries) {
        std::cin >> q;
    }

    std::cout << cache.IdealCaching(queries) << std::endl;
}