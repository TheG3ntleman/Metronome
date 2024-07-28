from src.problems.tower_of_hanoi import TowerOfHanoi
from src.problems.morpian_solitaire import MorpionSolitaire5D
from src.problems.sudoku import Sudoku

from src.solvers.FlatMonteCarlo import FlatMonteCarloSolver
from src.solvers.MultiArmBanditSolver import UCB1Solver
from src.solvers.NestedMonteCarlo import NMCTSSolver

from src.primitives.state_space_tree import StateSpaceTreeNode
from src.primitives.probe import Probe

from src.probes.SolverProbeAnalysis import SolverAnalysis

import numpy as np # type: ignore
import random

# Setting the seed for reproducibility
random.seed(10)

sudoku_3x3 = Sudoku(3, use_split_moves=False)
sudoku_4x4 = Sudoku(4, use_split_moves=False)
# tower_of_hanoi_3x3 = TowerOfHanoi(3, 3)
morpion_solitaire_5d = MorpionSolitaire5D(20)

fmc_solver = FlatMonteCarloSolver()
mab_solver = UCB1Solver(exploration_parameter=100)
nmc_solver = NMCTSSolver(depth=20)

root_node = StateSpaceTreeNode(None)
# probe = Probe(root_node, nmc_solver, morpion_solitaire_5d, 100)

# probe.probe()
# # probe.scatter_plot_rewards()
# probe.kernel_density_plots("results/morpion_solitaire_5d_kde/nmc")
# probe.save("results/morpion_solitaire_5d_kde/nmc")

# analysis = SolverAnalysis([fmc_solver, mab_solver, nmc_solver], sudoku_3x3, 10000)
analysis = SolverAnalysis([fmc_solver, mab_solver, nmc_solver], morpion_solitaire_5d, 5)


analysis.perform_analysis()


# analysis.kernel_density_plots(save_path = "analysis/sudoku_3x3_kde", show=True)
analysis.kernel_density_plots(save_path = "analysis/morpion_solitaire_5d/", show=True)

analysis.compare_statistics()