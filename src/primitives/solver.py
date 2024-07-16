import copy
import random
from src.primitives.state_space_tree import StateSpaceTreeNode

"""
Things to do:
    - Add optional print and plot support.
"""

class Solver:

    def __init__(self):
        pass

    def initialize_node(self, node : StateSpaceTreeNode):
        pass # Override this.

    def select_node(self, node : StateSpaceTreeNode):
        pass # Override this.

    def solve(self, problem):
        pass # Must return dictionary with {"success" : True/False, "solution" : "some solution", state_space_tree: root}

    @staticmethod
    def rollout(problem, node):
        # Can actually write a default
        # flat rollout mechanism here.
        problem_copy = copy.deepcopy(problem)

        while not problem_copy.is_game_finished():
            valid_moves = problem_copy.get_valid_moves()
            if len(valid_moves) == 0:
                return problem_copy.get_penalty() + problem_copy.get_score()
            move = random.choice(valid_moves)
            problem_copy.play_action(move)

        return problem_copy.get_score()
    
    def __str__(self):
        return "Solver"