from src.problems.tower_of_hanoi import TowerOfHanoi
from src.solvers.FlatMonteCarlo import FlatMonteCarloSolver
from src.solvers.MultiArmBanditSolver import UCB1Solver
from src.problems.sudoku import Sudoku

sudoku_3x3 = Sudoku([[0, 0, 0, 0, 6, 0, 0, 0, 8], [4, 0, 0, 0, 0, 0, 3, 0, 6], [0, 0, 0, 0, 0, 0, 7, 0, 0], [0, 2, 0, 0, 5, 0, 8, 0, 4], [3, 0, 0, 0, 1, 0, 0, 0, 0], [0, 6, 0, 0, 0, 0, 0, 9, 1], [0, 0, 0, 5, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 6, 0, 8, 0], [6, 0, 0, 0, 9, 0, 5, 0, 0]],
                    [[1, 7, 2, 3, 6, 5, 9, 4, 8], [4, 9, 8, 7, 2, 1, 3, 5, 6], [5, 3, 6, 9, 8, 4, 7, 1, 2], [9, 2, 1, 6, 5, 7, 8, 3, 4], [3, 8, 4, 2, 1, 9, 6, 7, 5], [7, 6, 5, 8, 4, 3, 2, 9, 1], [8, 1, 9, 5, 7, 2, 4, 6, 3], [2, 5, 7, 4, 3, 6, 1, 8, 9], [6, 4, 3, 1, 9, 8, 5, 2, 7]])
# tower_of_hanoi_3x3 = TowerOfHanoi(8, 4)


fmc_solver = FlatMonteCarloSolver()

# print(fmc_solver.solve(tower_of_hanoi_3x3,
#       selection_policy="robust_child", number_of_rollouts_per_decision=1000))


print(fmc_solver.solve(sudoku_3x3, number_of_rollouts_per_decision=1000, selection_policy="max_q_value_child"))
