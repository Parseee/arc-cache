#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "ARC/src/arc.hpp"

size_t hits;

static int slow_get_page(int num);

template <typename T>
int count_hits(size_t cache_size, size_t num_calls,
               const std::vector<T> &queries);

void process_files(int test_num);
int read_positive_int(const std::string &prompt = "") {
    int value;
    while (true) {
        if (!prompt.empty()) {
            std::cout << prompt;
        }
        std::cin >> value;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a valid integer."
                      << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (value <= 0) {
            std::cout << "Value must be positive. Please try again."
                      << std::endl;
        } else {
            break;
        }
    }
    return value;
}

int read_with_retry() {
    int value;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a valid integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
}

void read_queries(std::vector<int> &queries) {
    for (auto &q : queries) {
        q = read_with_retry();
    }
}

int main() {
    int cache_size = read_positive_int();
    int num_calls = read_positive_int();

    assert((cache_size > 0 && num_calls > 0) &&
           "cache size and num calls must be non-negative");

    std::vector<int> queries(num_calls);
    read_queries(queries);

    std::cout << count_hits(cache_size, num_calls, queries) << std::endl;
}

template <typename T>
int count_hits(size_t cache_size, size_t num_calls,
               const std::vector<T> &queries) {
    caches::Cache<int, int> cache(cache_size);
    hits = num_calls;
    for (const auto &q : queries) {
        cache.get_page(q, slow_get_page);
    }
    return hits;
}

int slow_get_page(int num) {
    --hits;
    return num * 2;
}