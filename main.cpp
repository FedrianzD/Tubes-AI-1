#include <iostream>
#include <chrono>
#include <thread>
#include "localsearch.cpp"
int main(){
    // cout << "Hello World" << endl;
    auto start = std::chrono::high_resolution_clock::now();

    problem p = problem();
    vector<vector<vector<int>>> result = steepestHillClimb(p);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    // std::vector<std::vector<std::vector<int>>> magic_cube = 
    // {
    //     {
    //         {22, 11, 9},
    //         {2, 27, 13},
    //         {18, 4, 20}
    //     },
    //     {
    //         {12, 7, 23},
    //         {25, 14, 3},
    //         {5, 21, 16}
    //     },
    //     {
    //         {8, 24, 10},
    //         {15, 1, 26},
    //         {19, 17, 6}
    //     }
    // };
    //   
    // cout << Util::Objective_Function(magic_cube, 3);

    for (int z = 0; z < 5; z++){
        for (int y = 0; y < 5; y++){
            for (int x = 0; x < 5; x++){
                cout << result[z][y][x] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}