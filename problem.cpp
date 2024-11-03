#include "util.cpp"
#include <tuple>
#include <future>
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

class A {
public:
    vector<int> state;
    int value;
    int n;

    A(vector<int> state, int n=5) : state(state), n(n) {
        this->value = Util::Objective_Function(state, n);
    }
};

class problem {
public:
    vector<int> current_state;
    vector<tuple<int, int, int>> coordinates;
    vector<pair<tuple<int, int, int>, tuple<int, int, int>>> all_pairs;
    int n;

    problem(int n = 5, int random_seed = 0) : n(n) {
        // Initialize with random state
        this->current_state = Util::randomize_initial_state(n, random_seed);

        // Generate all possible coordinates in the cube
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
                    this->coordinates.emplace_back(x, y, z);
                }
            }
        }

        // Generate pairs of coordinates
        for (size_t i = 0; i < this->coordinates.size(); ++i) {
            for (size_t j = i + 1; j < this->coordinates.size(); ++j) {
                this->all_pairs.emplace_back(this->coordinates[i], this->coordinates[j]);
            }
        }
    }

    int index(int z, int y, int x) const {
        return z * n * n + y * n + x;
    }

    vector<int> action(tuple<int, int, int> coor1, tuple<int, int, int> coor2) {
        vector<int> new_state = this->current_state;

        int temp = current_state[index(get<0>(coor1), get<1>(coor1), get<2>(coor1))];
        new_state[index(get<0>(coor1), get<1>(coor1), get<2>(coor1))] = current_state[index(get<0>(coor2), get<1>(coor2), get<2>(coor2))];
        new_state[index(get<0>(coor2), get<1>(coor2), get<2>(coor2))] = temp;

        return new_state;
    }

    A get_neighbor() {
        const int num_threads = thread::hardware_concurrency();
        int max_val = 0;
        vector<int> best_neighbor;

        auto chunk_size = all_pairs.size() / num_threads;
        vector<future<pair<int, vector<int>>>> futures;

        for (int i = 0; i < num_threads; ++i) {
            auto start = all_pairs.begin() + i * chunk_size;
            auto end = (i == num_threads - 1) ? all_pairs.end() : start + chunk_size;

            // Launch asynchronous task for each chunk
            futures.emplace_back(async(launch::async, [this, start, end]() {
                int local_max_val = 0;
                vector<int> local_best_neighbor;

                for (auto it = start; it != end; ++it) {
                    auto next_state = action(it->first, it->second);
                    int value = Util::Objective_Function(next_state, n);

                    if (value > local_max_val) {
                        local_max_val = value;
                        local_best_neighbor = next_state;
                    }
                }
                return make_pair(local_max_val, local_best_neighbor);
            }));
        }

        for (auto& fut : futures) {
            auto result = fut.get();
            if (result.first > max_val) {
                max_val = result.first;
                best_neighbor = result.second;
            }
        }

        current_state = best_neighbor;
    return A(best_neighbor);
    }
        A get_neighbor_random(){
            std::random_device rd;
            std::mt19937 rng(rd());
            std::shuffle(all_pairs.begin(), all_pairs.end(), rng);
            std::uniform_int_distribution<int> distInt(0, all_pairs.size()-1);
            auto pair = all_pairs[distInt(rng)];
            auto next_state = action(pair.first, pair.second);
            return A(next_state);
        }
};
