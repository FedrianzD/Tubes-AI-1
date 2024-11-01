from problem import *

# Steepest Ascent Hill-climbing
# Hill-climbing with Sideways Move
# Random Restart Hill-climbing
# Stochastic Hill-climbing
# Simulated Annealing
# Genetic Algorithm

def steepestHillCLimb(problem):
    current  = A(problem.current_state, problem.Objective_func)
    while True:
        neighbor = problem.get_neighbor()
        print(current.value)
        if neighbor.value <= current.value:
            print(BObjective_Function(current.state))
            return current.state
        current = neighbor

def HillCLimbSideways(problem):
    current  = A(problem.current_state)
    while True:
        neighbor = problem.get_neighbor()
        print(current.value)
        if neighbor.value < current.value:
            print(BObjective_Function(current.state))
            return current.state
        current = neighbor

def RandomRestratHillClimb():
    random_state = 0
    p = problem(random_state)
    current  = A(p.current_state)
    while current.value < 10:
        p = problem(random_state)
        random_state += 1
        current  = A(p.current_state)
        while True:
            neighbor = p.get_neighbor()
            if neighbor.value < current.value:
                break
            current = neighbor
        print(current.value)
    print("done")
    # return current.value

def StochasticHillCLimb(problem, iter):
    current  = A(problem.current_state)
    for i in range(iter):
        neighbor = problem.get_neighbor_random()
        print(neighbor.value, current.value)
        if neighbor.value > current.value:
            current = neighbor
    return current.state

def SimuatedAnnealing(problem, schedule, static=True, thresh=0.5):
    def choose_next(dE, T, thresh):
        proba = np.exp(dE / T)
        if not static:
            thresh = np.random.rand()
        return proba > thresh

    current = A(problem.current_state)
    t = 1
    while True:
        T = schedule.Calculate(t)
        if T == 0 :
            print(current.value)
            return current.state
        next_node = problem.get_neighbor_random()
        dE = next_node.value - current.value
        if dE > 0 :
            current = next_node
        else:
            if choose_next(dE, T, thresh):
                current = next_node
        t += 1

class SA_Scheduler:
    def __init__(self, tipe="linear", T0 = 100, alpha=0.99, beta = 0.1, k = 1):
        self.tipe = tipe
        self.T0 = T0
        self.alpha = alpha
        self.beta = beta
        self.k = k
    
    def Calculate(self, t):
        if self.tipe == "linear":
            return max(self.T0 - self.k * t, 0)
        elif self.tipe == "eksponen":
            return self.T0 * (self.alpha ** t)
        elif self.tipe == "log":
            return self.T0 / (1 + self.beta * np.log(1 + t))
        
class GeneticAlgo:
    def __init__(self, pop_size = 100, n=5, generations=1000, mutation_rate=0.1, crossover="ox"):
        self.pop_size = pop_size
        self.n = n
        self.generations = generations
        self.mutation_rate = mutation_rate
        if crossover == "ox":
            self.crossover = self.order_crossover
        elif crossover == "cx":
            self.crossover = self.cycle_crossover

    def init_population(self):
        n = self.n
        population = []
        for _ in range(self.pop_size):
            individual = np.random.permutation(n**3) + 1
            individual = individual.reshape((n, n, n))
            population.append(individual)
        return population
    
    def mutate(self, individual):
        n = self.n
        idx1 = tuple(np.random.randint(0, n, size=3))
        idx2 = tuple(np.random.randint(0, n, size=3))
        individual[idx1], individual[idx2] = individual[idx2], individual[idx1]
        return individual
    
    def order_crossover(self, parent1, parent2):
        n = parent1.shape[0]  # Cube size (e.g., 3 for a 3x3x3 cube)
        total_elements = n**3

        # Flatten the parents into 1D arrays
        parent1_flat = parent1.flatten()
        parent2_flat = parent2.flatten()

        # Randomly select two crossover points
        point1, point2 = sorted(np.random.choice(range(total_elements), 2, replace=False))

        # Create two children by taking the slice from parent1 and parent2
        child1_flat = np.zeros(total_elements)
        child2_flat = np.zeros(total_elements)

        # Child 1 gets a slice from parent1, Child 2 gets the same slice from parent2
        child1_flat[point1:point2] = parent1_flat[point1:point2]
        child2_flat[point1:point2] = parent2_flat[point1:point2]

        # Fill the rest of child1 from parent2 and child2 from parent1, avoiding duplicates
        p1_index = p2_index = 0
        for i in range(total_elements):
            if child1_flat[i] == 0:
                while parent2_flat[p2_index] in child1_flat:
                    p2_index += 1
                child1_flat[i] = parent2_flat[p2_index]
            
            if child2_flat[i] == 0:
                while parent1_flat[p1_index] in child2_flat:
                    p1_index += 1
                child2_flat[i] = parent1_flat[p1_index]

        # Reshape the flat children back into cubes
        child1 = np.array(child1_flat).reshape((n, n, n))
        child2 = np.array(child2_flat).reshape((n, n, n))

        return child1, child2

    def cycle_crossover(self, parent1, parent2):
        n = parent1.shape[0]
        total_elements = n**3

        # Flatten the parents into 1D arrays
        parent1_flat = parent1.flatten()
        parent2_flat = parent2.flatten()

        # Initialize the children with -1
        child1_flat = [-1] * total_elements
        child2_flat = [-1] * total_elements
        visited = [False] * total_elements

        # Start with the first unvisited index
        index = 0
        while -1 in child1_flat:
            if not visited[index]:
                start_index = index
                cycle_indices = []
                while True:
                    cycle_indices.append(index)
                    visited[index] = True
                    value = parent1_flat[index]
                    index = np.where(parent2_flat == value)[0][0]
                    if index == start_index:
                        break

                # Alternate cycles between parents
                if len([x for x in child1_flat if x != -1]) % 2 == 0:
                    # Fill this cycle from parent1 for child1, and from parent2 for child2
                    for i in cycle_indices:
                        child1_flat[i] = parent1_flat[i]
                        child2_flat[i] = parent2_flat[i]
                else:
                    # Fill this cycle from parent2 for child1, and from parent1 for child2
                    for i in cycle_indices:
                        child1_flat[i] = parent2_flat[i]
                        child2_flat[i] = parent1_flat[i]

            index = (index + 1) % total_elements

        child1 = np.array(child1_flat).reshape((n, n, n))
        child2 = np.array(child2_flat).reshape((n, n, n))

        return child1, child2

    def tournament_selection(self, population, fitnesses, k=3):
        selected = np.random.choice(range(len(population)), k, replace=False)
        best = max(selected, key=lambda i: fitnesses[i])
        return population[best]
    
    def genetic_algo(self):
        population = self.init_population()

        for generation in range(self.generations):
            fitnesses = [Objective_Function(ind) for ind in population]

            if max(fitnesses) == 105:
                print(f"Solution found at generation {generation}")
                return population[np.argmax(fitnesses)]
            
            new_population = []

            for _ in range(self.pop_size // 2):
                parent1 = self.tournament_selection(population, fitnesses)
                parent2 = self.tournament_selection(population, fitnesses)
                
                child1, child2 = self.crossover(parent1, parent2)
                
                if random.random() < self.mutation_rate:
                    child1 = self.mutate(child1)
                if random.random() < self.mutation_rate:
                    child2 = self.mutate(child2)
                
                new_population.append(child1)
                new_population.append(child2)
            
            population = new_population
            if (generation % 100 == 0):
                print("Generation", generation)
                print("Max fitness:", max(fitnesses))
        fitnesses = [Objective_Function(ind) for ind in population]
        return population[np.argmax(fitnesses)]