from src.problems.tower_of_hanoi import TowerOfHanoi
from src.solvers.FlatMonteCarlo import FlatMonteCarloSolver
from src.solvers.MultiArmBanditSolver import UCB1Solver

tower_of_hanoi_3x3 = TowerOfHanoi()
fmc_solver = UCB1Solver()

print(fmc_solver.solve(tower_of_hanoi_3x3, selection_policy="robust_child", number_of_rollouts_per_decision= 1000)) 