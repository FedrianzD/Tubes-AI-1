import numpy as np

def BObjective_Function(state, n=5):
    magic_number = (n * (n**3 + 1)) // 2
    fulfilled_properties = 0
    
    # kolom
    for i in range(n):
        for k in range(n):
            if np.sum(state[i, :, k]) == magic_number:
                fulfilled_properties += 1
    # tiang
    for j in range(n):
        for k in range(n):
            if np.sum(state[:, j, k]) == magic_number:
                fulfilled_properties += 1
    # baris
    for i in range(n):
        for j in range(n):
            if np.sum(state[i, j, :]) == magic_number:
                fulfilled_properties += 1
    # diagonal ruang
    if np.sum([state[i, i, i] for i in range(n)]) == magic_number:
        fulfilled_properties += 1
    if np.sum([state[i, i, n-i-1] for i in range(n)]) == magic_number:
        fulfilled_properties += 1
    if np.sum([state[i, n-i-1, i] for i in range(n)]) == magic_number:
        fulfilled_properties += 1
    if np.sum([state[i, n-i-1, n-i-1] for i in range(n)]) == magic_number:
        fulfilled_properties += 1
    
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

    return fulfilled_properties

def Objective_Function(state, n=5):
    magic_number = (n * (n**3 + 1)) // 2
    value = 0.0  # Start with float for accuracy in division

    # Columns
    for i in range(n):
        for k in range(n):
            value += 1 / (1 + np.abs(np.sum(state[i, :, k]) - magic_number))
    
    # Pillars
    for j in range(n):
        for k in range(n):
            value += 1 / (1 + np.abs(np.sum(state[:, j, k]) - magic_number))
    
    # Rows
    for i in range(n):
        for j in range(n):
            value += 1 / (1 + np.abs(np.sum(state[i, j, :]) - magic_number))
    
    # Spatial Diagonals
    value += 1 / (1 + np.abs(np.sum([state[i, i, i] for i in range(n)]) - magic_number))
    value += 1 / (1 + np.abs(np.sum([state[i, i, n-i-1] for i in range(n)]) - magic_number))
    value += 1 / (1 + np.abs(np.sum([state[i, n-i-1, i] for i in range(n)]) - magic_number))
    value += 1 / (1 + np.abs(np.sum([state[i, n-i-1, n-i-1] for i in range(n)]) - magic_number))

    # 2D Plane Diagonals (slices of the cube)
    for i in range(n):
        # XY
        value += 1 / (1 + np.abs(np.sum([state[j, j, i] for j in range(n)]) - magic_number))
        value += 1 / (1 + np.abs(np.sum([state[j, n-j-1, i] for j in range(n)]) - magic_number))
        
        # XZ
        value += 1 / (1 + np.abs(np.sum([state[j, i, j] for j in range(n)]) - magic_number))
        value += 1 / (1 + np.abs(np.sum([state[n-j-1, i, j] for j in range(n)]) - magic_number))
        
        # YZ
        value += 1 / (1 + np.abs(np.sum([state[i, j, j] for j in range(n)]) - magic_number))
        value += 1 / (1 + np.abs(np.sum([state[i, n-j-1, j] for j in range(n)]) - magic_number))

    return value

def Objective_Function(state, n=5):
    magic_number = (n * (n**3 + 1)) // 2
    value = 0.0  # Start with float for accuracy in division

    # Columns
    for i in range(n):
        for k in range(n):
            value += 1 / (1 + np.abs(np.sum(state[i, :, k]) - magic_number))
    
    # Pillars
    for j in range(n):
        for k in range(n):
            value += 1 / (1 + np.abs(np.sum(state[:, j, k]) - magic_number))
    
    # Rows
    for i in range(n):
        for j in range(n):
            value += 1 / (1 + np.abs(np.sum(state[i, j, :]) - magic_number))
    
    # Spatial Diagonals
    value += 1 / (1 + np.abs(np.sum([state[i, i, i] for i in range(n)]) - magic_number))
    value += 1 / (1 + np.abs(np.sum([state[i, i, n-i-1] for i in range(n)]) - magic_number))
    value += 1 / (1 + np.abs(np.sum([state[i, n-i-1, i] for i in range(n)]) - magic_number))
    value += 1 / (1 + np.abs(np.sum([state[i, n-i-1, n-i-1] for i in range(n)]) - magic_number))

    # 2D Plane Diagonals (slices of the cube)
    for i in range(n):
        # XY
        value += 1 / (1 + np.abs(np.sum([state[j, j, i] for j in range(n)]) - magic_number))
        value += 1 / (1 + np.abs(np.sum([state[j, n-j-1, i] for j in range(n)]) - magic_number))
        
        # XZ
        value += 1 / (1 + np.abs(np.sum([state[j, i, j] for j in range(n)]) - magic_number))
        value += 1 / (1 + np.abs(np.sum([state[n-j-1, i, j] for j in range(n)]) - magic_number))
        
        # YZ
        value += 1 / (1 + np.abs(np.sum([state[i, j, j] for j in range(n)]) - magic_number))
        value += 1 / (1 + np.abs(np.sum([state[i, n-j-1, j] for j in range(n)]) - magic_number))

    return value

def Objective_Function_Symmetry(state, n=5):
    magic_number = (n * (n**3 + 1)) // 2
    deviation_sum = 0
    
    # Penalty term for symmetry across all rows, columns, and diagonals
    penalties = 0
    
    # Columns
    for i in range(n):
        for k in range(n):
            col_sum = np.sum(state[i, :, k])
            deviation_sum += (col_sum - magic_number) ** 2  # Penalize deviation from magic number
            penalties += np.var(state[i, :, k])  # Variance penalty for symmetry

    # Pillars
    for j in range(n):
        for k in range(n):
            pillar_sum = np.sum(state[:, j, k])
            deviation_sum += (pillar_sum - magic_number) ** 2
            penalties += np.var(state[:, j, k])

    # Rows
    for i in range(n):
        for j in range(n):
            row_sum = np.sum(state[i, j, :])
            deviation_sum += (row_sum - magic_number) ** 2
            penalties += np.var(state[i, j, :])

    # Spatial Diagonals
    diag_sums = [
        np.sum([state[i, i, i] for i in range(n)]),
        np.sum([state[i, i, n-i-1] for i in range(n)]),
        np.sum([state[i, n-i-1, i] for i in range(n)]),
        np.sum([state[i, n-i-1, n-i-1] for i in range(n)])
    ]
    for diag_sum in diag_sums:
        deviation_sum += (diag_sum - magic_number) ** 2
    penalties += np.var(diag_sums)

    # Objective function that combines deviation from magic number with symmetry penalty
    value = deviation_sum + penalties * 0.1  # Weight the penalty term to balance influence

    return -value

def Objective_Function_Threshold(state, n=5):
    magic_number = (n * (n**3 + 1)) // 2
    reward = 0

    # Function to assign rewards based on thresholds
    def threshold_reward(deviation):
        if deviation == 0:
            return 3  # Highest reward if sum matches magic number
        elif deviation <= 2:
            return 2  # Medium reward if close
        elif deviation <= 5:
            return 1  # Low reward if somewhat close
        else:
            return 0  # No reward if far from target

    # Calculate rewards for rows, columns, and diagonals
    # Columns
    for i in range(n):
        for k in range(n):
            col_sum = np.sum(state[i, :, k])
            reward += threshold_reward(abs(col_sum - magic_number))

    # Pillars
    for j in range(n):
        for k in range(n):
            pillar_sum = np.sum(state[:, j, k])
            reward += threshold_reward(abs(pillar_sum - magic_number))

    # Rows
    for i in range(n):
        for j in range(n):
            row_sum = np.sum(state[i, j, :])
            reward += threshold_reward(abs(row_sum - magic_number))

    # Spatial Diagonals
    diag_sums = [
        np.sum([state[i, i, i] for i in range(n)]),
        np.sum([state[i, i, n-i-1] for i in range(n)]),
        np.sum([state[i, n-i-1, i] for i in range(n)]),
        np.sum([state[i, n-i-1, n-i-1] for i in range(n)])
    ]
    for diag_sum in diag_sums:
        reward += threshold_reward(abs(diag_sum - magic_number))

    return reward

def Objective_Function_Exponential(state, n=5):
    magic_number = (n * (n**3 + 1)) // 2
    total_penalty = 0
    k = 0.1  # Scale factor for exponential penalty

    # Columns
    for i in range(n):
        for k in range(n):
            col_sum = np.sum(state[i, :, k])
            deviation = abs(col_sum - magic_number)
            total_penalty += np.exp(k * deviation)

    # Pillars
    for j in range(n):
        for k in range(n):
            pillar_sum = np.sum(state[:, j, k])
            deviation = abs(pillar_sum - magic_number)
            total_penalty += np.exp(k * deviation)

    # Rows
    for i in range(n):
        for j in range(n):
            row_sum = np.sum(state[i, j, :])
            deviation = abs(row_sum - magic_number)
            total_penalty += np.exp(k * deviation)

    # Spatial Diagonals
    diag_sums = [
        np.sum([state[i, i, i] for i in range(n)]),
        np.sum([state[i, i, n-i-1] for i in range(n)]),
        np.sum([state[i, n-i-1, i] for i in range(n)]),
        np.sum([state[i, n-i-1, n-i-1] for i in range(n)])
    ]
    for diag_sum in diag_sums:
        deviation = abs(diag_sum - magic_number)
        total_penalty += np.exp(k * deviation)

    return total_penalty

def Objective_Function_Dynamic(state, n=5, iteration=1, alpha=0.01):
    magic_number = (n * (n**3 + 1)) // 2
    total_penalty = 0
    scale_factor = 1 + alpha * iteration  # Increase penalty over iterations

    # Columns
    for i in range(n):
        for k in range(n):
            col_sum = np.sum(state[i, :, k])
            deviation = (col_sum - magic_number) ** 2
            total_penalty += deviation * scale_factor

    # Pillars
    for j in range(n):
        for k in range(n):
            pillar_sum = np.sum(state[:, j, k])
            deviation = (pillar_sum - magic_number) ** 2
            total_penalty += deviation * scale_factor

    # Rows
    for i in range(n):
        for j in range(n):
            row_sum = np.sum(state[i, j, :])
            deviation = (row_sum - magic_number) ** 2
            total_penalty += deviation * scale_factor

    # Spatial Diagonals
    diag_sums = [
        np.sum([state[i, i, i] for i in range(n)]),
        np.sum([state[i, i, n-i-1] for i in range(n)]),
        np.sum([state[i, n-i-1, i] for i in range(n)]),
        np.sum([state[i, n-i-1, n-i-1] for i in range(n)])
    ]
    for diag_sum in diag_sums:
        deviation = (diag_sum - magic_number) ** 2
        total_penalty += deviation * scale_factor

    return total_penalty

def randomize_initial_state(n=5, random_state=0):
    rng = np.random.default_rng(random_state)
    
    # Generate array of numbers from 1 to n^3
    numbers = np.arange(1, n**3 + 1)
    
    # Shuffle the numbers using the RNG
    rng.shuffle(numbers)
    
    # Reshape the shuffled numbers into an n x n x n array
    state = numbers.reshape((n, n, n))
    print(state)
    
    return state