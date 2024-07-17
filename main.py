from src.problems.tower_of_hanoi import TowerOfHanoi
from src.problems.morpian_solitaire import MorpionSolitaire5D
from src.solvers.FlatMonteCarlo import FlatMonteCarloSolver
from src.solvers.FlatMonteCarloAgain import FlatMonteCarloAgain
from src.solvers.MultiArmBanditSolver import UCB1Solver
from src.problems.sudoku import Sudoku

# sudoku_3x3 = Sudoku([[2, 8, 1, 4, 0, 9, 7, 0, 6], [9, 0, 4, 5, 7, 6, 8, 1, 2], [6, 0, 0, 1, 0, 2, 3, 0, 9], [4, 6, 3, 0, 2, 5, 9, 8, 0], [5, 2, 7, 8, 0, 1, 6, 0, 0], [0, 9, 0, 0, 6, 4, 2, 7, 5], [8, 0, 9, 6, 5, 3, 1, 2, 7], [7, 1, 0, 9, 4, 8, 5, 6, 3], [3, 5, 6, 2, 0, 0, 0, 9, 8]])

sudoku_3x3 = Sudoku([[2, 8, 1, 4, 0, 9, 7, 0, 6], [9, 0, 4, 5, 7, 6, 8, 1, 2], [6, 0, 0, 1, 0, 2, 3, 0, 9], [4, 6, 3, 0, 2, 5, 9, 8, 0], [5, 2, 7, 8, 0, 1, 6, 0, 0], [0, 9, 0, 0, 6, 4, 2, 7, 5], [8, 0, 9, 6, 5, 3, 1, 2, 7], [7, 1, 0, 9, 4, 8, 5, 6, 3], [3, 5, 6, 2, 0, 0, 0, 9, 8]])
# tower_of_hanoi_3x3 = TowerOfHanoi(3, 3)


# morpion_solitaire_5d = MorpionSolitaire5D(5)

fmc_solver = FlatMonteCarloSolver()
mab_solver = UCB1Solver(exploration_parameter=1.5)

output = mab_solver.solve(
        sudoku_3x3,
        selection_policy="max_q_value_child",
        number_of_rollouts_per_decision=5000,
)
print(output)
# morpion_solitaire.plot()
# print(morpion_solitaire.get_score())
# output["state_space_tree"].plot()


