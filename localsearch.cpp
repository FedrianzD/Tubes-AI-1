#include "problem.cpp"
#include <iostream>

vector<vector<vector<int>>> steepestHillClimb(problem p){
    A current = A(p.current_state);
    while (true){
        A neighbor = p.get_neighbor();
        // cout << neighbor.value << endl;
        if (neighbor.value <= current.value){
            cout << current.value << endl;
            return current.state;
        }
        current = neighbor;
    }
}
class Scheduler{
    public:
        string tipe;
        int T0;
        double alpha;
        double beta;
        double k;
        Scheduler(string tipe = "linear", double T0 = 100, double alpha = 0.99, double beta = 0.1, double k = 1){
            this->tipe = tipe;
            this->T0 = T0;
            this->alpha = alpha;
            this->beta = beta;
            this->k = k;
        }
        double calculate(int time){
            if (tipe == "linear"){
                return max(T0 - k * time, 0.0);
            } else if (tipe == "exp"){
                return T0 * pow(alpha, time);
            } else { // log
                return T0 / (1 + beta * log(time + 1));
            }
        }

};

bool choose_next(double delta, double T, bool static2 = true, double thresh = 0.5){
    double proba = exp(delta/T);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> distFloat(0.0f, 1.0f);
    if (!static2){
        thresh = distFloat(rng);
    }
    return proba > thresh;
}
vector<vector<vector<int>>> simulatedAnnealing(problem p, Scheduler scheduler, double thresh = 0.5){
        
    A current = A(p.current_state);
    int time = 1;
    while (true){
        double T = scheduler.calculate(time);
        if (T == 0){
            return current.state;
        }
        A next = p.get_neighbor_random();
        int delta = next.value - current.value;
        if (delta > 0){
            current = next;
        } else {
            if (choose_next(delta, T, true, thresh)){
                current = next;
            }
        }
        cout << "T: " << T << endl;
        cout << "value: " << current.value << endl;
        time++;
    }
}