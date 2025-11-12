#pragma once

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace caches {
template <typename T> class BeladyCache final {
    using iterator_t = typename std::vector<T>::const_iterator;

  public:
    explicit BeladyCache<T>(size_t cache_size)
        : cache_size_(cache_size), hits_(0) {
        cache_.reserve(cache_size_);
    }

    BeladyCache(const BeladyCache &) = delete;
    BeladyCache &operator=(const BeladyCache &) = delete;

    int IdealCaching(const std::vector<T> &queries) {
        hits_ = 0;
        for (auto it = queries.begin(); it != queries.end(); ++it) {
            future_positions_[*it].push(it);
        }

        for (auto it = queries.begin(); it != queries.end(); ++it) {
            future_positions_[*it].pop();

            if (cache_.contains(*it)) {
                ++hits_;
                continue;
            }

            if (future_positions_[*it].empty()) {
                continue;
            }

            if (cache_.size() < cache_size_) {
                cache_.insert(*it);
            } else {
                HandleMiss(it, queries);
            }
        }
        return hits_;
    }

  private:
    void HandleMiss(iterator_t it, const std::vector<T> &queries) {
        auto farthest_query = it;
        auto victim = cache_.end();

        for (auto cache_it = cache_.begin(); cache_it != cache_.end();
             ++cache_it) {
            if (future_positions_[*cache_it].empty()) {
                farthest_query = queries.end();
                victim = cache_it;
                break;
            } else {
                auto next_use = future_positions_[*cache_it].front();
                if (next_use > farthest_query) {
                    victim = cache_it;
                    farthest_query = next_use;
                }
            }
        }
        if (victim == cache_.end()) {
            std::cerr << "failed to find victim\n";
        }
        cache_.erase(victim);
        cache_.insert(*it);
    }

    std::unordered_set<T> cache_;
    std::unordered_map<T, std::queue<iterator_t>> future_positions_;
    size_t cache_size_;
    size_t hits_ = 0;
};
} // namespace caches
