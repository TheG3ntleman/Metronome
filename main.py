from src.problems.tower_of_hanoi import TowerOfHanoi
from src.problems.morpian_solitaire import MorpionSolitaire5D
from src.solvers.FlatMonteCarlo import FlatMonteCarloSolver
from src.solvers.MultiArmBanditSolver import UCB1Solver
from src.solvers.NestedMonteCarlo import NMCTSSolver
from src.problems.sudoku import Sudoku

import numpy as np # type: ignore
import random

# Setting the seed for reproducibility
random.seed(10)

# sudoku_3x3 = Sudoku([[2, 8, 1, 4, 0, 9, 7, 0, 6], [9, 0, 4, 5, 7, 6, 8, 1, 2], [6, 0, 0, 1, 0, 2, 3, 0, 9], [4, 6, 3, 0, 2, 5, 9, 8, 0], [5, 2, 7, 8, 0, 1, 6, 0, 0], [0, 9, 0, 0, 6, 4, 2, 7, 5], [8, 0, 9, 6, 5, 3, 1, 2, 7], [7, 1, 0, 9, 4, 8, 5, 6, 3], [3, 5, 6, 2, 0, 0, 0, 9, 8]])

sudoku_3x3 = Sudoku(3, use_split_moves=False)
# tower_of_hanoi_3x3 = TowerOfHanoi(3, 3)


morpion_solitaire_5d = MorpionSolitaire5D(20)

fmc_solver = FlatMonteCarloSolver()
mab_solver = UCB1Solver(exploration_parameter=100)
nm_solver = NMCTSSolver()

output = nm_solver.solve(
        morpion_solitaire_5d,
        selection_policy="max_q_value_child",
        number_of_rollouts_per_decision=1000,
)
print(output)
# morpion_solitaire.plot()
# print(morpion_solitaire.get_score())
# output["state_space_tree"].plot()

"""
His Majesty, the king sudoku,
[4 6 0 8 3 1 0 0 0]
[7 2 9 5 6 4 1 8 0]
[1 0 8 9 0 7 4 0 6]
[8 1 6 3 7 9 5 2 4]
[0 4 2 6 0 8 9 3 7]
[9 0 3 2 4 5 0 6 1]
[0 8 0 0 9 3 2 7 0]
[0 5 7 4 8 0 3 0 0]
[0 9 1 0 5 2 0 4 0]
"""


