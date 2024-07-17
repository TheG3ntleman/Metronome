from src.problems.tower_of_hanoi import TowerOfHanoi
from src.problems.morpian_solitaire import MorpionSolitaire5D
from src.problems.sudoku import Sudoku

from src.solvers.FlatMonteCarlo import FlatMonteCarloSolver
from src.solvers.MultiArmBanditSolver import UCB1Solver

from src.primitives.state_space_tree import StateSpaceTreeNode
from src.primitives.probe import Probe

import numpy as np
import random

# Setting the seed for reproducibility
random.seed(10)

sudoku_3x3 = Sudoku(3, use_split_moves=True)
# tower_of_hanoi_3x3 = TowerOfHanoi(3, 3)
# morpion_solitaire_5d = MorpionSolitaire5D(5)

fmc_solver = FlatMonteCarloSolver()
mab_solver = UCB1Solver(exploration_parameter=100)

root_node = StateSpaceTreeNode(None)
probe = Probe(root_node, fmc_solver, sudoku_3x3, 850)
probe.probe()
probe.scatter_plot_rewards()