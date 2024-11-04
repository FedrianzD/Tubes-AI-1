#include <iostream>
#include <chrono>
#include <thread>
#include "localsearch.cpp"

int main(){
    auto start = chrono::high_resolution_clock::now();

    problem p = problem(5, 0);
    Scheduler scheduler = Scheduler("eksponen", 100, 0.9999, 0.1, 0.1);
    // vector<vector<vector<int>>> result = Util::convertTo3D(RandomRestartHillClimbing(), 5, 5, 5);
    // vector<vector<vector<int>>> result = Util::convertTo3D(steepestHillClimbGLS(p).state, 5, 5, 5);
    // vector<vector<vector<int>>> result = Util::convertTo3D(steepestHillClimb(p).state, 5, 5, 5);
    vector<vector<vector<int>>> result = Util::convertTo3D(simulatedAnnealing(p, scheduler));
    // vector<vector<vector<int>>> result = Util::convertTo3D(simulatedAnnealingGLS(p, scheduler));
    // GeneticAlgo gen = GeneticAlgo();
    // vector<vector<vector<int>>> result = Util::convertTo3D(gen.genetic_algo(),5,5,5);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;

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