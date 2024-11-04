#include <iostream>
#include <chrono>
#include <thread>
#include "localsearch.cpp"

int main(){
    auto start = chrono::high_resolution_clock::now();
    int algo;
    cerr << "Masukkan algoritma yang ingin digunakan (1. SteepestHillClimbing, 2. SidewaysHillClimb, 3. RandomRestartHillClimbing, 4. StochasticHillClimb, 5. SimulatedAnnealing, 6. GeneticAlgo): ";
    cin >> algo;
    problem p = problem(5, 0);
    vector<vector<vector<int>>> result;
    Scheduler scheduler;
    string tipe;
    double T0;
    double alpha;
    double beta;
    double k;
    GeneticAlgo gen;
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
        cerr << "Masukkan jumlah iterasi: " << endl;
        int iter;
        cin >> iter;
        result = Util::convertTo3D(StochasticHillClimb(p, iter), 5, 5, 5);
        break;
    case 5:
        cerr << "Masukkan tipe scheduler (linear, exp, log): ";
        
        cin >> tipe;
        cerr << "Masukkan T0: ";
        
        cin >> T0;
        cerr << "Masukkan alpha: ";
        
        cin >> alpha;
        cerr << "Masukkan beta: ";
        
        cin >> beta;
        cerr << "Masukkan k: ";
        
        cin >> k;
        scheduler = Scheduler(tipe, T0, alpha, beta, k);
        result = Util::convertTo3D(simulatedAnnealing(p, scheduler), 5, 5, 5);
        break;
    case 6:
        gen = GeneticAlgo();
        result = Util::convertTo3D(gen.genetic_algo(),5,5,5);
        break;
    default:
        break;
    }
    
    // vector<vector<vector<int>>> result = Util::convertTo3D(RandomRestartHillClimbing(), 5, 5, 5);
    // vector<vector<vector<int>>> result = Util::convertTo3D(steepestHillClimbGLS(p).state, 5, 5, 5);
    // vector<vector<vector<int>>> result = Util::convertTo3D(steepestHillClimb(p).state, 5, 5, 5);
    // vector<vector<vector<int>>> result = Util::convertTo3D(simulatedAnnealing(p, scheduler));
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