#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Util {
public:
    static int Objective_Function(vector<int>& state, int n = 5) {
        int magic_number = (n * (n * n * n + 1)) / 2;
        int fulfilled_properties = 0;

        auto get = [&](int z, int y, int x) {
            return state[z * n * n + y * n + x];
        };

        // Check Columns
        for (int z = 0; z < n; ++z) {
            for (int x = 0; x < n; ++x) {
                int col_sum = 0;
                for (int y = 0; y < n; ++y) {
                    col_sum += get(z, y, x);
                }
                if (col_sum == magic_number) ++fulfilled_properties;
            }
        }

        // Check Rows
        for (int z = 0; z < n; ++z) {
            for (int y = 0; y < n; ++y) {
                int row_sum = 0;
                for (int x = 0; x < n; ++x) {
                    row_sum += get(z, y, x);
                }
                if (row_sum == magic_number) ++fulfilled_properties;
            }
        }

        // Check Layers
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                int layer_sum = 0;
                for (int z = 0; z < n; ++z) {
                    layer_sum += get(z, y, x);
                }
                if (layer_sum == magic_number) ++fulfilled_properties;
            }
        }

        // Check Space Diagonals
        int space_diagonals[4] = {0};
        for (int i = 0; i < n; ++i) {
            space_diagonals[0] += get(i, i, i);
            space_diagonals[1] += get(i, i, n - i - 1);
            space_diagonals[2] += get(i, n - i - 1, i);
            space_diagonals[3] += get(i, n - i - 1, n - i - 1);
        }
        for (int i = 0; i < 4; ++i) {
            if (space_diagonals[i] == magic_number) ++fulfilled_properties;
        }

        for (int i = 0; i < n; ++i) {
            // XY plane
            int xy_diag1 = 0, xy_diag2 = 0;
            for (int j = 0; j < n; ++j) {
                xy_diag1 += get(j, j, i);
                xy_diag2 += get(j, n - j - 1, i);
            }
            if (xy_diag1 == magic_number) ++fulfilled_properties;
            if (xy_diag2 == magic_number) ++fulfilled_properties;

            // XZ plane
            int xz_diag1 = 0, xz_diag2 = 0;
            for (int j = 0; j < n; ++j) {
                xz_diag1 += get(j, i, j);
                xz_diag2 += get(n - j - 1, i, j);
            }
            if (xz_diag1 == magic_number) ++fulfilled_properties;
            if (xz_diag2 == magic_number) ++fulfilled_properties;

            // YZ plane
            int yz_diag1 = 0, yz_diag2 = 0;
            for (int j = 0; j < n; ++j) {
                yz_diag1 += get(i, j, j);
                yz_diag2 += get(i, n - j - 1, j);
            }
            if (yz_diag1 == magic_number) ++fulfilled_properties;
            if (yz_diag2 == magic_number) ++fulfilled_properties;
        }

        return fulfilled_properties;
    }

    static vector<int> randomize_initial_state(int n = 5, int random_state = 0) {
        // vector<vector<vector<int>>> temp = 
        //     {
        //         {
        //             {69, 123, 37, 56, 97},
        //             {14, 2, 76, 12, 38},
        //             {17, 119, 6, 24, 51},
        //             {86, 102, 93, 35, 40},
        //             {54, 103, 99, 21, 85}
        //         },
        //         {
        //             {89, 11, 28, 113, 107},
        //             {45, 65, 91, 53, 9},
        //             {10, 101, 23, 20, 67},
        //             {68, 95, 5, 26, 94},
        //             {106, 118, 100, 108, 73}
        //         },
        //         {
        //             {4, 75, 84, 43, 16},
        //             {31, 3, 72, 36, 83},
        //             {82, 44, 18, 92, 29},
        //             {19, 7, 66, 115, 25},
        //             {88, 58, 22, 81, 87}
        //         },
        //         {
        //             {116, 48, 1, 61, 71},
        //             {63, 46, 111, 27, 52},
        //             {50, 98, 117, 120, 41},
        //             {62, 13, 124, 33, 122},
        //             {90, 47, 59, 15, 74}
        //         },
        //         {
        //             {39, 109, 32, 112, 105},
        //             {49, 78, 77, 8, 110},
        //             {104, 64, 125, 70, 114},
        //             {79, 60, 55, 30, 42},
        //             {57, 121, 34, 80, 96}
        //         }
        //     };
        // vector<int> state = convertTo1D(temp);
        vector<int> state(n * n * n);
        iota(state.begin(), state.end(), 1);

        std::mt19937 rng(random_state);
        std::shuffle(state.begin(), state.end(), rng);

        return state;
    }

    static vector<vector<vector<int>>> convertTo3D(const vector<int>& vec1D, int n = 5, int m = 5, int p = 5) {
        vector<vector<vector<int>>> vec3D(n, vector<vector<int>>(m, vector<int>(p)));
        
        if (vec1D.size() != n * m * p) {
            throw invalid_argument("Ukuran vector 1D tidak sesuai dengan dimensi yang diberikan.");
        }
        
        int index = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                for (int k = 0; k < p; ++k) {
                    vec3D[i][j][k] = vec1D[index++];
                }
            }
        }
        
        return vec3D;
    }

    static vector<int> convertTo1D(const vector<vector<vector<int>>>& vec3D) {
        vector<int> vec1D;
        for (const auto& matrix : vec3D) {
            for (const auto& row : matrix) {
                for (int value : row) {
                    vec1D.push_back(value);
                }
            }
        }
        return vec1D;
    }
};
