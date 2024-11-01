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