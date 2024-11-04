#include "problem.cpp"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <future>
#include <condition_variable>
#include <algorithm>
#include <random>
#include <numeric>

using namespace std;

tuple<int, int, int> reverseIndex(int index, int n) {
    int z = index / (n * n);
    int y = (index / n) % n;
    int x = index % n;
    return make_tuple(z, y, x);
}

void print_point(const tuple<int, int, int>& point) {
    cout << get<0>(point) << " " << get<1>(point) << " " << get<2>(point) << " ";
}

pair<tuple<int, int, int>, tuple<int, int, int>> getPoint(vector<int> state1, vector<int> state2) {
    int point1 = -1;
    int point2 = -1;
    for (int i=0; i < state1.size(); i++) {
        if (state1[i] != state2[i]) {
            if (point1 != -1) {
                point2 = i;
            } else {
                point1 = i;
            }
        }
    }
    return make_pair(reverseIndex(point1, 5), reverseIndex(point2, 5));
}

Node steepestHillClimb(problem p) {
    cout << "HC" << endl;
    Node current = Node(p.current_state);
    int i = 0;
    cout << i << endl;
    cout << current.value << endl;
    for(int j = 0; j < current.state.size(); j++){
                cout << current.state[j] << " ";
    }
    cout << endl;
    while (true) {
        i++;
        Node neighbor = p.get_neighbor();
        if (neighbor.value <= current.value) {
            return current;
        }
        cout << i << endl;
        pair<tuple<int, int, int>, tuple<int, int, int>> points = getPoint(neighbor.state, current.state);
        print_point(points.first);
        print_point(points.second);
        cout << endl;
        current = neighbor;
        cout << current.value << endl;
    }
}

Node SidewaysHillClimb(problem p, int max_sideways) {
    cout << "SMHC" << endl;
    Node current = Node(p.current_state);
    int i = 0;
    cout << i << endl;
    cout << current.value << endl;
    for(int j = 0;j < current.state.size(); j++){
                cout << current.state[j] << " ";
    }
    int sideways = 0;
    while (true && sideways < max_sideways) {
        i++;
        cout << i << endl;
        Node neighbor = p.get_neighbor();
        if (neighbor.value < current.value) {
            return current;
        }
        current = neighbor;
        cout << current.value << endl;
    }
    return current;
}

vector<int> RandomRestartHillClimbing() {
    mutex output_mutex;
    const int num_threads = thread::hardware_concurrency();
    int best_value = 0;
    vector<int> best_state;

    for (int batch_start = 0; best_value < 60; batch_start += num_threads) {
        vector<future<Node>> futures;

        for (int i = 0; i < num_threads; ++i) {
            int random_seed = batch_start + i;
            problem p = problem(5, random_seed);
            futures.emplace_back(async(launch::async, steepestHillClimb, p));
        }

        for (auto& fut : futures) {
            auto result = fut.get();
            int value = result.value;

            if (value > best_value) {
                lock_guard<mutex> lock(output_mutex);
                best_value = value;
                best_state = result.state;
            }
        }

        lock_guard<mutex> lock(output_mutex);
        cout << "Best value after " << batch_start + num_threads
             << " random restarts: " << best_value << endl;
    }

    cout << "Final best value: " << best_value << endl;
    return best_state;
}

vector<int> StochasticHillClimb(problem p, int iter){
    Node current = Node(p.current_state);  
    for (int i = 0; i < iter; ++i) {
        Node neighbor = p.get_neighbor_random(); 
        if (neighbor.value > current.value) {
            current = neighbor;
        }
    }
    return current.state;  
}

class GeneticAlgo {
public:
    int pop_size, n, generations;
    double mutation_rate;
    string crossover_type;

    GeneticAlgo(int pop_size = 100, int n = 5, int generations = 1000, double mutation_rate = 0.1, string crossover_type = "ox")
        : pop_size(pop_size), n(n), generations(generations), mutation_rate(mutation_rate), crossover_type(crossover_type) {}

    vector<int> init_individual() {
        vector<int> individual(n * n * n);
        iota(individual.begin(), individual.end(), 1);
        shuffle(individual.begin(), individual.end(), random_device());
        return individual;
    }

    vector<int> mutate(vector<int> individual) {
        int idx1 = rand() % individual.size();
        int idx2 = rand() % individual.size();
        swap(individual[idx1], individual[idx2]);
        return individual;
    }

    pair<vector<int>, vector<int>> order_crossover(const vector<int>& parent1, const vector<int>& parent2) {
        int total_elements = n * n * n;
        vector<int> child1(total_elements, 0), child2(total_elements, 0);

        int point1 = rand() % total_elements, point2 = rand() % total_elements;
        if (point1 > point2) swap(point1, point2);

        copy(parent1.begin() + point1, parent1.begin() + point2, child1.begin() + point1);
        copy(parent2.begin() + point1, parent2.begin() + point2, child2.begin() + point1);

        for (int i = 0; i < total_elements; ++i) {
            if (child1[i] == 0) {
                for (int val : parent2) {
                    if (find(child1.begin(), child1.end(), val) == child1.end()) {
                        child1[i] = val;
                        break;
                    }
                }
            }
            if (child2[i] == 0) {
                for (int val : parent1) {
                    if (find(child2.begin(), child2.end(), val) == child2.end()) {
                        child2[i] = val;
                        break;
                    }
                }
            }
        }

        return {child1, child2};
    }

    vector<int> tournament_selection(const vector<vector<int>>& population, const vector<double>& fitnesses, int k = 3) {
        auto max_fitness_it = max_element(fitnesses.begin(), fitnesses.end());
        int max_index = distance(fitnesses.begin(), max_fitness_it);
        return population[max_index];
    }

    vector<int> genetic_algo() {
        vector<vector<int>> population;
        for (int i = 0; i < pop_size; ++i) {
            population.push_back(init_individual());
        }

        vector<double> fitnesses;
        for (int generation = 0; generation < generations; ++generation) {
            // Parallel fitness computation
            vector<future<double>> fitness_futures;
            for (auto& individual : population) {
                fitness_futures.push_back(async(launch::async, [&individual, this]() { return Util::Objective_Function(individual, n); }));
            }

            fitnesses.clear();
            for (auto& f : fitness_futures) {
                fitnesses.push_back(f.get());
            }

            if (*max_element(fitnesses.begin(), fitnesses.end()) == 105) {
                cout << "Solution found at generation " << generation << endl;
                return population[max_element(fitnesses.begin(), fitnesses.end()) - fitnesses.begin()];
            }

            vector<vector<int>> new_population;
            for (int i = 0; i < pop_size / 2; ++i) {
                // Select parents
                vector<int> parent1 = tournament_selection(population, fitnesses);
                vector<int> parent2 = tournament_selection(population, fitnesses);

                auto [child1, child2] = order_crossover(parent1, parent2);

                if ((double)rand() / RAND_MAX < mutation_rate) child1 = mutate(child1);
                if ((double)rand() / RAND_MAX < mutation_rate) child2 = mutate(child2);

                new_population.push_back(child1);
                new_population.push_back(child2);
            }

            population = new_population;

            if (generation % 100 == 0) {
                cout << "Generation " << generation << ", Max fitness: " << *max_element(fitnesses.begin(), fitnesses.end()) << endl;
            }
        }

        auto best_it = max_element(fitnesses.begin(), fitnesses.end());
        return population[best_it - fitnesses.begin()];
    }
    
};

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
    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution<float> distFloat(0.0f, 1.0f);
    if (!static2){
        thresh = distFloat(rng);
    }
    // cout << (proba > thresh) << endl;
    cout << proba << endl;
    return proba > thresh;
}
vector<int> simulatedAnnealing(problem p, Scheduler scheduler, double thresh = 0.5){
    int cont = 0;
    Node current = Node(p.current_state);
    cout << "SA" << endl;
    int i = 0;
    cout << i << endl;
    cout << current.value << endl;
    for(int j = 0; j < current.state.size(); j++){
                cout << current.state[j] << " ";
    }
    cout << endl;
    int time = 1;
    while (true){
        i++;
        cout << "iterasi: " << i << endl;
        double T = scheduler.calculate(time);
        if (T == 0){
            return current.state;
        }
        cout << i << endl;
        Node next = p.get_neighbor_random();
        int delta = next.value - current.value;
        if (delta > 0){
            cout << "1" << endl;
            pair<tuple<int, int, int>, tuple<int, int, int>> points = getPoint(next.state, current.state);
            print_point(points.first);
            print_point(points.second);
            cout << endl;
            current = next;
        } else {
            cont++;
            if (choose_next(delta, T, true, thresh)){
                pair<tuple<int, int, int>, tuple<int, int, int>> points = getPoint(next.state, current.state);
                print_point(points.first);
                print_point(points.second);
                cout << endl;
                current = next;
            } else {
                cout << "-1 -1 -1 -1 -1 -1" << endl;
            }
        }
        cout << current.value << endl;
        // cout << "t: " << time << endl;
        // cout << "value: " << current.value << endl;
        // if (time % 10000 == 0) {
        //     cout << "t: " << time << endl;
        //     cout << "value: " << current.value << endl;
        // }
        time++;
        cout << "stuck: " << cont << endl;
    }
}