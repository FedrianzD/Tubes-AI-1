import numpy as np

def Base_Objective_Function(state, n=5):
    magic_number = (n * (n**3 + 1)) // 2
    fulfilled_properties = 0
    
    # kolom
    for i in range(n):
        for k in range(n):
            if np.sum(state[i, :, k]) == magic_number:
                fulfilled_properties += 1
    # print("Baris", fulfilled_properties)
    # tiang
    for j in range(n):
        for k in range(n):
            if np.sum(state[:, j, k]) == magic_number:
                fulfilled_properties += 1
    # print("kolom", fulfilled_properties)
    # baris
    for i in range(n):
        for j in range(n):
            if np.sum(state[i, j, :]) == magic_number:
                fulfilled_properties += 1
    # print("tiang", fulfilled_properties)
    # diagonal ruang
    if np.sum([state[i, i, i] for i in range(n)]) == magic_number:
        fulfilled_properties += 1
    if np.sum([state[i, i, n-i-1] for i in range(n)]) == magic_number:
        fulfilled_properties += 1
    if np.sum([state[i, n-i-1, i] for i in range(n)]) == magic_number:
        fulfilled_properties += 1
    if np.sum([state[i, n-i-1, n-i-1] for i in range(n)]) == magic_number:
        fulfilled_properties += 1
    # print("diagonal ruang", fulfilled_properties)
    # diagonal potongan bidang
    for i in range(n):
        # XY
        if np.sum([state[j, j, i] for j in range(n)]) == magic_number:
            fulfilled_properties += 1
        if np.sum([state[j, n-j-1, i] for j in range(n)]) == magic_number:
            fulfilled_properties += 1
        
        # XZ
        if np.sum([state[j, i, j] for j in range(n)]) == magic_number:
            fulfilled_properties += 1
        if np.sum([state[n-j-1, i, j] for j in range(n)]) == magic_number:
            fulfilled_properties += 1
        
        # YZ
        if np.sum([state[i, j, j] for j in range(n)]) == magic_number:
            fulfilled_properties += 1
        if np.sum([state[i, n-j-1, j] for j in range(n)]) == magic_number:
            fulfilled_properties += 1
    # print("diagonal potongan bidang", fulfilled_properties)

    return fulfilled_properties

def randomize_initial_state(n=5, random_state=0):
    rng = np.random.default_rng(random_state)
    
    # Generate array of numbers from 1 to n^3
    numbers = np.arange(1, n**3 + 1)
    
    # Shuffle the numbers using the RNG
    rng.shuffle(numbers)
    
    # Reshape the shuffled numbers into an n x n x n array
    state = numbers.reshape((n, n, n))
    
    return state

def analyze_successors(current_val, successors):
    print("worse:", len([x for x in successors if x < current_val]))
    print("same:", len([x for x in successors if x == current_val]))
    print("better:", len([x for x in successors if x > current_val]))

def diff(cube1, cube2):
    return cube1 != cube2

def Objective_Function(solution, penalties, lambda_param):
    costs = [Base_Objective_Function(solution) for _ in range(109)]
    utilities = calculate_utility(costs, penalties)
    if utilities:
        max_utility_feature = max(utilities)[1]
        penalties[max_utility_feature] += 1
    base_cost = Base_Objective_Function(solution)
    penalty_term = lambda_param * sum(penalties[i] for i in range(109))
    return base_cost + penalty_term

def calculate_utility(costs, penalties):
    utilities = []
    for i in range(109):
        utility = costs[i] / (1 + penalties[i])
        utilities.append((utility, i))
    return utilities