from localsearch import *
from problem import *
from util import *
import time

p = problem(random_state=0, Objective_Func=Objective_Function)
scheduler = SA_Scheduler(tipe="eksponen", T0=100000, alpha=0.99999)

start_time = time.time()

# SimuatedAnnealing(p, scheduler)
# steepestHillCLimb(p) 
# HillCLimbSideways(p)
gen = GeneticAlgo(generations=1000)
gen.genetic_algo()

end_time = time.time()
execution_time = end_time - start_time
print(f"Execution time: {execution_time} seconds")