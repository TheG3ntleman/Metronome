from src.primitives.solver import Solver
from src.primitives.state_space_tree import StateSpaceTreeNode
from tqdm import tqdm  # type: ignore
import random
import math
from copy import deepcopy

class NMCTSSolver(Solver):
    def __init__(self, exploration_parameter=1 / math.sqrt(2), depth=2):
        super().__init__()
        self.exploration_parameter = exploration_parameter
        self.depth = depth
        self.ucb_formula = lambda r, n, N: (
            r / n
        ) + self.exploration_parameter * math.sqrt(math.log(N) / n)

    def solve(self, problem, number_of_rollouts_per_decision=1000, selection_policy="max_q_value_child"):
        problem.pprint()
        # Initializing the state space tree
        root = StateSpaceTreeNode(None)
        root.properties["visits"] = 0
        root.properties["reward"] = 0

        current_node = root
        moves_played = 0

        while not problem.is_game_finished():
            # STEP 1 : Getting the valid moves at this state
            valid_moves = problem.get_valid_moves()

            # Checking if there are any valid moves
            if len(valid_moves) == 0:
                # This means that we have failed to find a valid
                # solution. Since NMCTS has no backtracking, this
                # becomes a failed run. However, instead of returning no
                # solution we return what we ended up choosing at each step.
                return {"success": False, "solution": current_node.get_actions(), "state_space_tree": root}

            # If there is only one valid move, play it directly
            if len(valid_moves) == 1:
                current_node = StateSpaceTreeNode(valid_moves[0], current_node)
                current_node.properties["visits"] = 0
                current_node.properties["reward"] = 0
                problem.play_action(current_node.action)
                moves_played += 1
                print("Playing action number:", moves_played)
                problem.pprint()
                continue

            # STEP 3 : Expanding node
            for move in valid_moves:
                child = StateSpaceTreeNode(move, current_node)
                child.properties["visits"] = 0
                child.properties["reward"] = 0
                current_node.add_child(child)

            # STEP 4: Performing rollouts to make decisions
            bestChild = None
            bestScore = -float('inf')

            for child in current_node.children:
                # Performing nested rollout
                reward = self.nested_rollout(problem, child, self.depth)
                
                if reward > bestScore:
                    bestChild = child
                    bestScore = reward

                child.properties["reward"] += reward
                child.properties["visits"] += 1

                # Cheap backpropagation
                current_node.properties["reward"] += reward
                current_node.properties["visits"] += 1

            # STEP 5: Applying the best action
            if bestChild:
                current_node = bestChild
                problem.play_action(current_node.action)
                moves_played += 1
                print("Playing action number:", moves_played)
                problem.pprint()

        return {"success": True, "solution": current_node.get_actions(), "state_space_tree": root}

    def nested_rollout(self, problem, node, depth):
        """
        Perform a nested rollout at the specified depth level.
        """
        if depth == 0 or problem.is_game_finished():
            return self.evaluate(problem)

        # Perform a rollout from the given node and move
        copy_problem = deepcopy(problem)
        copy_problem.play_action(node.action)
        valid_moves = copy_problem.get_valid_moves()

        if len(valid_moves) == 0:
            return self.evaluate(copy_problem)

        # Perform rollouts for each valid move at the next depth level
        bestScore = -float('inf')
        for nextMove in valid_moves:
            copy_copy_problem = deepcopy(copy_problem)
            copy_copy_problem.play_action(nextMove)
            score = self.nested_rollout(copy_copy_problem, node, depth - 1)
            bestScore = max(bestScore, score)

        return bestScore

    def evaluate(self, problem):
        """
        Evaluate the problem state and return a reward value.
        """
        return problem.get_score()

    def __str__(self):
        return "Nested Monte Carlo Tree Search Solver"
