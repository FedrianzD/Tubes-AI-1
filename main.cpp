#include <iostream>
#include <chrono>
#include <thread>
#include "localsearch.cpp"

int main(){
    auto start = chrono::high_resolution_clock::now();

    problem p = problem(5, 10);
    // vector<vector<vector<int>>> result = Util::convertTo3D(RandomRestartHillClimbing(), 5, 5, 5);
    Scheduler scheduler = Scheduler("linear", 100, 0.99, 0.1, 0.1);
    vector<vector<vector<int>>> result = simulatedAnnealing(p, scheduler);
    // GeneticAlgo gen = GeneticAlgo();
    // vector<vector<vector<int>>> result = Util::convertTo3D(gen.genetic_algo(),5,5,5);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    // vector<vector<vector<int>>> magic_cube = 
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