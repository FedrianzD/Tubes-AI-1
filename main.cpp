#include <iostream>
#include <chrono>
#include <thread>
#include "localsearch.cpp"

int main(){
    auto start = chrono::high_resolution_clock::now();

    problem p = problem(5, 10);
    // vector<vector<vector<int>>> result = Util::convertTo3D(RandomRestartHillClimbing(), 5, 5, 5);
    Scheduler scheduler = Scheduler("linear", 10000000, 0.99, 0.1, 0.1);
    vector<int> result = simulatedAnnealing(&p, scheduler);
    // GeneticAlgo gen = GeneticAlgo();
    // vector<vector<vector<int>>> result = Util::convertTo3D(gen.genetic_algo(),5,5,5);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    return 0;
}