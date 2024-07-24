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
            for _ in tqdm(range(number_of_rollouts_per_decision)):
                # Choosing a random child
                # child = current_node.children[random.randint(0, len(current_node.children) - 1)]

                # Checking if there is a child that has not been visited
                unvisited_children = [
                    child for child in current_node.children if child.properties["visits"] == 0]
                if len(unvisited_children) > 0:
                    child = unvisited_children[0]
                else:
                    # Choosing the child with the highest UCB1 value
                    ucb_values = [self.ucb_formula(child.properties["reward"], child.properties["visits"],
                                                   current_node.properties["visits"]) for child in current_node.children]
                    child = current_node.children[ucb_values.index(
                        max(ucb_values))]

                # Performing nested rollout
                reward = self.nested_rollout(problem, child, self.depth)

                child.properties["reward"] += reward
                child.properties["visits"] += 1

                # Cheap backpropagation
                current_node.properties["reward"] += reward
                current_node.properties["visits"] += 1

            # STEP 5: Choosing some policy by using methods proposed by Chaslot.

            for i, child in enumerate(current_node.children):
                print(f"{i}. Action: {child.action} Reward: {child.properties['reward']} Visits: {child.properties['visits']}")

            selection_index = 0
            if selection_policy == "max_q_value_child":
                q_values = [child.properties["reward"] / child.properties["visits"] if child.properties["visits"] != 0 else 0
                            for child in current_node.children]
                selection_index = q_values.index(max(q_values))
            elif selection_policy == "max_child":
                rewards = [child.properties["reward"]
                           for child in current_node.children]
                selection_index = rewards.index(max(rewards))
            elif selection_policy == "robust_child":
                visits = [child.properties["visits"]
                          for child in current_node.children]
                selection_index = visits.index(max(visits))
            current_node = current_node.children[selection_index]

            # STEP 6: Applying the action
            current_node.properties["visits"] = 0
            problem.play_action(current_node.action)
            moves_played += 1
            print("Playing action number:", moves_played,
                  "whose index is:", selection_index)
            problem.pprint()

        return {"success": True, "solution": current_node.get_actions(), "state_space_tree": root}

    def nested_rollout(self, problem, node, depth, current_depth=0):
        """
        Perform a nested rollout at the specified depth level.
        """
        if current_depth > depth:
            # Perform random rollouts
            while not problem.is_game_finished():
                valid_moves = problem.get_valid_moves()
                if len(valid_moves) == 0:
                    return problem.get_penalty() + problem.get_score()
                move = random.choice(valid_moves)
                problem.play_action(move)

            return problem.get_score()
        else:
            # Perform nested rollouts
            if problem.is_game_finished():
                return self.evaluate(problem)

            # Copy the current state to simulate rollouts
            problem_copy = deepcopy(problem)

            # Get valid moves
            valid_moves = problem_copy.get_valid_moves()
            if not valid_moves:
                return self.evaluate(problem_copy)

            move = random.choice(valid_moves)
            # Evaluate each child node by performing rollouts
            best_reward = float('-inf')
            """
            for move in valid_moves:
                # Copy the state for each move
                state_copy = deepcopy(problem_copy)
                state_copy.play_action(move)

                # Create a child node
                child_node = StateSpaceTreeNode(move, node)
                child_node.properties["visits"] = 0
                child_node.properties["reward"] = 0
                node.add_child(child_node)

                # Perform recursive nested rollouts
                reward = self.nested_rollout(
                    state_copy, child_node, depth, current_depth + 1)

                # Track the best reward from the child nodes
                if reward > best_reward:
                    best_reward = reward
            """
                # Copy the state for each move
            state_copy = deepcopy(problem_copy)
            state_copy.play_action(move)

            # Create a child node
            child_node = StateSpaceTreeNode(move, node)
            child_node.properties["visits"] = 0
            child_node.properties["reward"] = 0
            node.add_child(child_node)

            # Perform recursive nested rollouts
            reward = self.nested_rollout(
                state_copy, child_node, depth, current_depth + 1)

            # Track the best reward from the child nodes
            if reward > best_reward:
                best_reward = reward
            return best_reward

    def evaluate(self, problem):
        """
        Evaluate the problem state and return a reward value.
        """
        return problem.get_score()

    def __str__(self):
        return "Nested Monte Carlo Tree Search Solver"
