#include "util.cpp"
#include <iostream>

using namespace std;

class A{
    public:
    vector<vector<vector<int>>> state;
    int value;
    A(vector<vector<vector<int>>> state){
        this->state = state;
        this->value = Util::Objective_Function(state);
    }
};

class problem{
    public:
        vector<vector<vector<int>>> current_state;
        std::vector<std::tuple<int, int, int>> coordinates;
        vector<pair<tuple<int, int, int>, tuple<int, int, int>>> all_pairs;
        problem(){

            // Initialize with random state 
            this->current_state = Util::randomize_initial_state();

            // Generate all possible coordinates in the cube
            for (int x = 0; x < 5; ++x) {
                for (int y = 0; y < 5; ++y) {
                    for (int z = 0; z < 5; ++z) {
                        this->coordinates.emplace_back(x, y, z);
                    }
                }
            }

            // Generate pair of coordinates
            for (size_t i = 0; i < this->coordinates.size(); ++i) {
                for (size_t j = i + 1; j < this->coordinates.size(); ++j) {
                    this->all_pairs.emplace_back(this->coordinates[i], this->coordinates[j]);
                }
            }
        }

        vector<vector<vector<int>>> action(tuple<int, int, int> coor1, tuple<int, int, int> coor2){
            vector<vector<vector<int>>> new_state = this->current_state;

            int temp = current_state[get<0>(coor1)][get<1>(coor1)][get<2>(coor1)];

            new_state[get<0>(coor1)][get<1>(coor1)][get<2>(coor1)] = current_state[get<0>(coor2)][get<1>(coor2)][get<2>(coor2)];

            new_state[get<0>(coor2)][get<1>(coor2)][get<2>(coor2)] = temp;
            return new_state;
        }

        A get_neighbor(){
            int max_val = 0;
            std::vector<std::vector<std::vector<int>>> neighbor;

            std::random_device rd;
            std::mt19937 rng(rd());
            std::shuffle(all_pairs.begin(), all_pairs.end(), rng);

            for (const auto pair : all_pairs) {
                auto next_state = action(pair.first, pair.second);
                int value = Util::Objective_Function(next_state);
                
                if (value > max_val) {
                    max_val = value;
                    neighbor = next_state;
                }
            }

                current_state = neighbor;
                return A(neighbor);
            }
};


