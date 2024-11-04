#include <iostream>
#include <chrono>
#include <thread>
#include "localsearch.cpp"

int main(){
    auto start = chrono::high_resolution_clock::now();
    int algo;
    cout << "Masukkan algoritma yang ingin digunakan (1. SteepestHillClimbing, 2. SidewaysHillClimb, 3. RandomRestartHillClimbing, 4. StochasticHillClimb, 5. SimulatedAnnealing, 6. GeneticAlgo): ";
    cin >> algo;
    problem p = problem(5, 0);
    vector<vector<vector<int>>> result;
    Scheduler scheduler;
    switch (algo)
    {
    case 1:
        result = Util::convertTo3D(steepestHillClimb(p).state, 5, 5, 5);
        break;
    case 2:
        result = Util::convertTo3D(SidewaysHillClimb(p).state, 5, 5, 5);
        break;
    case 3:
        result = Util::convertTo3D(RandomRestartHillClimbing(), 5, 5, 5);
        break;
    case 4:
        result = Util::convertTo3D(stochasticHillClimb(p).state, 5, 5, 5);
        break;
    case 5:
        cout << "Masukkan tipe scheduler (linear, exp, log): ";
        string tipe;
        cin >> tipe;
        cout << "Masukkan T0: ";
        double T0;
        cin >> T0;
        cout << "Masukkan alpha: ";
        double alpha;
        cin >> alpha;
        cout << "Masukkan beta: ";
        double beta;
        cin >> beta;
        cout << "Masukkan k: ";
        double k;
        cin >> k;
        scheduler = Scheduler(tipe, T0, alpha, beta, k);
        result = Util::convertTo3D(simulatedAnnealing(p, scheduler), 5, 5, 5);
        break;
    case 6:
        GeneticAlgo gen = GeneticAlgo();
        result = Util::convertTo3D(gen.genetic_algo(),5,5,5);
        break;
    default:
        break;
    }
    
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