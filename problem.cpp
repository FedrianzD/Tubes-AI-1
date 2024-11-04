#include "util.cpp"
#include <tuple>
#include <future>
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

class Node {
public:
    vector<int> state;
    double value;
    int n;

    Node(vector<int> state, int n=5) : state(state), n(n) {
        this->value = Util::BObjective_Function(state, n);
    }
};

class problem {
public:
    vector<int> current_state;
    vector<tuple<int, int, int>> coordinates;
    vector<pair<tuple<int, int, int>, tuple<int, int, int>>> all_pairs;
    double lambda = 0.1;
    vector<int> penalties;  // Changed to vector
    int n;
    int iteration = 0;
    // vector<pair<vector<int>, int>> val_iter;

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
                penalties.push_back(0); // Initialize penalties for each pair
            }
        }
    }

    int index(int z, int y, int x) const {
        return z * n * n + y * n + x;
    }

    tuple<int, int, int> reverseIndex(int index, int n) const {
        int z = index / (n * n);
        int y = (index / n) % n;
        int x = index % n;
        return make_tuple(z, y, x);
    }

    vector<int> action(tuple<int, int, int> coor1, tuple<int, int, int> coor2) {
        vector<int> new_state = this->current_state;

        int temp = current_state[index(get<0>(coor1), get<1>(coor1), get<2>(coor1))];
        new_state[index(get<0>(coor1), get<1>(coor1), get<2>(coor1))] = current_state[index(get<0>(coor2), get<1>(coor2), get<2>(coor2))];
        new_state[index(get<0>(coor2), get<1>(coor2), get<2>(coor2))] = temp;

        return new_state;
    }

    double guided_objective_function(vector<int>& state, const pair<tuple<int, int, int>, tuple<int, int, int>>& action) {
        int obj_value = Util::BObjective_Function(state);

        // Calculate the index for the penalty in the penalties vector
        auto it = find(all_pairs.begin(), all_pairs.end(), action);
        int index = distance(all_pairs.begin(), it);

        int penalty = penalties[index];
        return obj_value - lambda * penalty;
    }

    Node get_neighbor() {
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
                    int value = Util::BObjective_Function(next_state, n);

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
        return Node(best_neighbor);
    }

    Node get_neighbor_random() {
        const int current_val = Util::BObjective_Function(current_state);
        // double better = 0;
        // double same = 0;
        // double worse = 0;
        for (const auto& pair : all_pairs) {
            // Launch asynchronous task for each pair in all_pairs
            auto next_state = action(pair.first, pair.second);
            int value = Util::BObjective_Function(next_state, n);
            // if (value > current_val) {
            //     better++;
            // } else if (value == current_val) {
            //     same++;
            // } else {
            //     worse++;
            // }
        }
        // cout << "worse: " << worse/all_pairs.size() << ", same: " << same/all_pairs.size() << ", better: " << better/all_pairs.size() << endl;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, n - 1);

        tuple<int, int, int> point1, point2;

        do {
            point1 = make_tuple(distrib(gen), distrib(gen), distrib(gen));
            point2 = make_tuple(distrib(gen), distrib(gen), distrib(gen));
        } while (point1 == point2);  

        auto next_state = action(point1, point2);
        // for (int i = 0; i < next_state.size(); i++) {
        //     if (next_state[i] != current_state[i]) {
        //         cout << next_state[i] << " ";
        //         cout << current_state[i] << " ";
        //         cout << i << endl;
        //         cout << Util::BObjective_Function(current_state) << " ";
        //         cout << Util::BObjective_Function(next_state) << endl;
        //     }
        // }

        return Node(next_state);
    }



    void update_penalties(const vector<int>& state) {
        for (size_t i = 0; i < all_pairs.size(); ++i) {
            const auto& [coor1, coor2] = all_pairs[i];

            if (state[index(get<0>(coor1), get<1>(coor1), get<2>(coor1))] != 
                state[index(get<0>(coor2), get<1>(coor2), get<2>(coor2))]) {
                penalties[i]++;
            }
        }
    }
};