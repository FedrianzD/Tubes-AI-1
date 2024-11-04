from util import *
import itertools
import random

class A:
    def __init__(self, state, Objective_func):
        self.state = state
        self.value = Objective_func(state) 

class problem:
    def __init__(self, Objective_Func, random_state=0):
        self.current_state = randomize_initial_state(random_state=random_state)
        coordinates = list(itertools.product(range(0, 5), repeat=3))
        self.all_pairs = [(coor1, coor2) for coor1, coor2 in itertools.combinations(coordinates, 2)]
        self.Objective_func = Objective_Func

    def action(self, coor1, coor2):
        next_state = self.current_state.copy()
        temp = next_state[coor1]
        next_state[coor1] = next_state[coor2]
        next_state[coor2] = temp
        return next_state

    def get_neighbor(self):
        state_values = [(self.action(pair[0], pair[1]), self.Objective_func(self.action(pair[0], pair[1]))) for pair in self.all_pairs]
        state_values = [(self.action(pair[0], pair[1]), self.Objective_func(self.action(pair[0], pair[1]))) for pair in self.all_pairs]
        sorted_states = sorted(state_values, key=lambda x: x[1], reverse=True)

        max_val = sorted_states[0][1]

        max_states = [state[0] for state in sorted_states if state[1] == max_val]

        neighbor = max_states[0]

        self.current_state = neighbor
        
        return A(neighbor, self.Objective_func)

    def get_neighbor_random(self):
        # pair1 = [random.randint(0, 4) for _ in range(3)]
        # pair2 = [random.randint(0, 4) for _ in range(3)]
        # random.shuffle(self.all_pairs)
        index = random.randint(0, len(self.all_pairs) - 1)
        next_state = self.action(self.all_pairs[index][0], self.all_pairs[index][1])
        # next_state = self.action(pair1, pair2)
        self.current_state = next_state
        return A(next_state)
    
    def analyze_proba(self):
        state_values = [(self.action(pair[0], pair[1]), self.Objective_func(self.action(pair[0], pair[1]))) for pair in self.all_pairs]
        values = [x[1] for x in state_values]
        cur_val = self.Objective_func(self.current_state)
        print("val:",cur_val)
        # analyze_successors(cur_val, values)

state = randomize_initial_state()
perfect_magic_cube_3x3 = np.array([
    [[22, 11, 9], [2, 27, 13], [18, 4, 20]],
    [[12, 7, 23], [25, 14, 3], [5, 21, 16]],
    [[8, 24, 10], [15, 1, 26], [19, 17, 6]]
])