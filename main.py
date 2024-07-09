from src.problems.tower_of_hanoi import TowerOfHanoi
from src.problems.morpian_solitaire import MorpionSolitaire5D
from src.solvers.FlatMonteCarlo import FlatMonteCarloSolver
from src.solvers.MultiArmBanditSolver import UCB1Solver

tower_of_hanoi_3x3 = TowerOfHanoi()
morpion_solitaire = MorpionSolitaire5D(20)

fmc_solver = FlatMonteCarloSolver()
mab_solver = UCB1Solver()

output = mab_solver.solve(
        morpion_solitaire,
        selection_policy="robust_child",
        number_of_rollouts_per_decision=3,
)

morpion_solitaire.plot()
print(morpion_solitaire.get_score())
output["state_space_tree"].plot()