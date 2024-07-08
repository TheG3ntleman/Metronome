import copy
import random

class Solver:

    def __init__(self):
        pass

    def solve(self, problem):
        pass # Must return dictionary with {"success" : True/False, "solution" : "some solution", state_space_tree: root}

    def rollout(self, problem, node):
        # Can actually write a default
        # flat rollout mechanism here.
        problem_copy = copy.deepcopy(problem)

        while not problem_copy.is_game_finished():
            valid_moves = problem_copy.get_valid_moves()
            if len(valid_moves) == 0:
                return problem_copy.get_penalty() + problem_copy.get_score()
            problem_copy.play_action(random.choice(valid_moves))

        
        return problem_copy.get_score()
        
    def __str__(self):
        return "Solver"