from src.problems.tower_of_hanoi import TowerOfHanoi
from src.problems.morpian_solitaire import MorpionSolitaire5D
from src.solvers.FlatMonteCarlo import FlatMonteCarloSolver
from src.solvers.FlatMonteCarloAgain import FlatMonteCarloAgain
from src.solvers.MultiArmBanditSolver import UCB1Solver
from src.problems.sudoku import Sudoku

sudoku_3x3 = Sudoku([[0, 0, 0, 0, 0, 0, 9, 8, 4], [0, 1, 5, 0, 9, 4, 0, 0, 0], [9, 4, 0, 0, 6, 2, 0, 5, 0], [1, 3, 0, 0, 4, 5, 2, 6, 0], [4, 5, 0, 0, 0, 8, 1, 0, 3], [0, 8, 0, 7, 0, 0, 4, 9, 0], [5, 0, 0, 2, 8, 0, 3, 0, 6], [0, 0, 2, 1, 3, 6, 0, 4, 7], [0, 0, 0, 4, 0, 0, 0, 2, 9]])
# tower_of_hanoi_3x3 = TowerOfHanoi(3, 3)


# morpion_solitaire_5d = MorpionSolitaire5D(5)

fmc_solver = FlatMonteCarloSolver()
mab_solver = UCB1Solver(exploration_parameter=1.5)

output = fmc_solver.solve(
        morpion_solitaire,
        selection_policy="robust_child",
        number_of_rollouts_per_decision=20,
)

morpion_solitaire.plot()
print(morpion_solitaire.get_score())
output["state_space_tree"].plot()


