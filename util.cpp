#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Util {
    public:
        static int Objective_Function(vector<vector<vector<int>>> vector, int n = 5){
            int magic_number = (n * (n*n*n + 1)) / 2;
            // cout << magic_number << endl;
            int fulfilled_properties = 0;

            // x = kolom, y = baris, z = lapisan

            // Check Columns
            for(int z=0; z < n; z++){
                for(int x=0; x < n; x++){
                    int sum = 0;
                    for(int y=0; y < n; y++){
                        sum +=  vector[z][y][x];
                    }
                    if(sum == magic_number){
                        fulfilled_properties++;
                        // cout << z << " " << y << " " << x << endl;
                    }
                }
            }

            // Check Rows
            for(int z=0; z < n; z++){
                for(int y=0; y < n; y++){
                    int sum = 0;
                    for(int x=0; x < n; x++){
                        sum +=  vector[z][y][x];
                    }
                    if(sum == magic_number){
                        fulfilled_properties++;
                        // cout << z << " " << y << " " << x << endl;
                    }
                    
                }
            }

            // Check Layers
            for(int x=0; x < n; x++){
                for(int y=0; y < n; y++){
                    int sum = 0;
                    for(int z=0; z < n; z++){
                        sum +=  vector[z][y][x];
                    }
                    if(sum == magic_number){
                        fulfilled_properties++;
                        // cout << z << " " << y << " " << x << endl;
                    }
                    
                }
            }

            // Check Space Diagonals
            int sum1 = 0;
            int sum2 = 0;
            int sum3 = 0;
            int sum4 = 0;
            for(int i = 0; i < n; i++){
                sum1 += vector[i][i][i];
                sum2 += vector[i][i][n-i-1];
                sum3 += vector[i][n-i-1][i];
                sum4 += vector[i][n-i-1][n-i-1];
            }
            if(sum1 == magic_number){
                fulfilled_properties++;
            }
            if(sum2 == magic_number){
                fulfilled_properties++;
            }
            if(sum3 == magic_number){
                fulfilled_properties++;
            }
            if(sum4 == magic_number){
                fulfilled_properties++;
            }

            // Check Diagonals
            for(int i = 0; i < n; i++){
                // YZ Plane Diagonals
                int sum1 = 0, sum2 = 0;
                for (int j = 0; j < n; j++) {
                    sum1 += vector[j][j][i];            
                    sum2 += vector[j][n - j - 1][i];    
                }
                if (sum1 == magic_number) fulfilled_properties++;
                if (sum2 == magic_number) fulfilled_properties++;

                // ZX Plane Diagonals
                sum1 = 0; sum2 = 0;
                for (int j = 0; j < n; j++) {
                    sum1 += vector[j][i][j];            
                    sum2 += vector[n - j - 1][i][j];    
                }
                if (sum1 == magic_number) fulfilled_properties++;
                if (sum2 == magic_number) fulfilled_properties++;

                // YX Plane Diagonals
                sum1 = 0; sum2 = 0;
                for (int j = 0; j < n; j++) {
                    sum1 += vector[i][j][j];            
                    sum2 += vector[i][n - j - 1][j];   
                }
                if (sum1 == magic_number) fulfilled_properties++;
                if (sum2 == magic_number) fulfilled_properties++;

            }
            return fulfilled_properties;
        }

        static vector<vector<vector<int>>> randomize_initial_state(int n = 5, int random_state = 22)
        {

            vector<vector<vector<int>>> state = 
            {
                {
                    {69, 123, 37, 56, 97},
                    {14, 2, 76, 12, 38},
                    {17, 119, 6, 24, 51},
                    {86, 102, 93, 35, 40},
                    {54, 103, 99, 21, 85}
                },
                {
                    {89, 11, 28, 113, 107},
                    {45, 65, 91, 53, 9},
                    {10, 101, 23, 20, 67},
                    {68, 95, 5, 26, 94},
                    {106, 118, 100, 108, 73}
                },
                {
                    {4, 75, 84, 43, 16},
                    {31, 3, 72, 36, 83},
                    {82, 44, 18, 92, 29},
                    {19, 7, 66, 115, 25},
                    {88, 58, 22, 81, 87}
                },
                {
                    {116, 48, 1, 61, 71},
                    {63, 46, 111, 27, 52},
                    {50, 98, 117, 120, 41},
                    {62, 13, 124, 33, 122},
                    {90, 47, 59, 15, 74}
                },
                {
                    {39, 109, 32, 112, 105},
                    {49, 78, 77, 8, 110},
                    {104, 64, 125, 70, 114},
                    {79, 60, 55, 30, 42},
                    {57, 121, 34, 80, 96}
                }
            };
            // state.resize(n, vector<vector<int>>(n, vector<int>(n)));


            // std::mt19937 rng(random_state);

            // int size = n * n * n;
            // std::vector<int> numbers(size);
            // for (int i = 0; i < size; ++i) {
            //     numbers[i] = i + 1;  
            // }
            // std::shuffle(numbers.begin(), numbers.end(), rng);

            // int index = 0;
            // for(int z=0; z < n; z++){
            //     for(int x=0; x < n; x++){
            //         for(int y=0; y < n; y++){
            //             state[z][y][x] = numbers[index++];
            //         }
            //     }
            // }
            return state;
        }
};