from src.primitives.solver import Solver
from src.primitives.state_space_tree import StateSpaceTreeNode
from tqdm import tqdm #type: ignore
import random
import math

class SPMultiArmBanditSolver(Solver):

    def __init__(self, exploration_parameter=1 / math.sqrt(2), d_parameter=1.0):
        super().__init__()
        self.exploration_parameter = exploration_parameter
        self.d_parameter = d_parameter
        self.ucb_formula = lambda r, n, N, sum_x_squared: (
            (r / n) +
            self.exploration_parameter * math.sqrt(math.log(N) / n) +
            math.sqrt((sum_x_squared - (n * (r / n)**2) + self.d_parameter) / n)
        )
    
    def solve(self, problem, number_of_rollouts_per_decision=1000, selection_policy="max_q_value_child"):
        problem.pprint()
        root = StateSpaceTreeNode(None)
        root.properties["visits"] = 0
        root.properties["reward"] = 0
        root.properties["sum_x_squared"] = 0

        current_node = root 
        moves_played = 0
        
        while not problem.is_game_finished():
            valid_moves = problem.get_valid_moves()
            if len(valid_moves) == 0:
                return {"success": False, "solution": current_node.get_actions(), "state_space_tree": root}
            
            if len(valid_moves) == 1:
                current_node = StateSpaceTreeNode(valid_moves[0], current_node)
                current_node.properties["visits"] = 0
                current_node.properties["reward"] = 0
                current_node.properties["sum_x_squared"] = 0
                problem.play_action(current_node.action)
                moves_played += 1
                print("Playing action number:", moves_played)
                problem.pprint()
                continue

            for move in valid_moves:
                child = StateSpaceTreeNode(move, current_node)
                child.properties["visits"] = 0
                child.properties["reward"] = 0
                child.properties["sum_x_squared"] = 0
                current_node.add_child(child)

            for _ in tqdm(range(number_of_rollouts_per_decision)):
                unvisited_children = [child for child in current_node.children if child.properties["visits"] == 0]
                if len(unvisited_children) > 0:
                    child = unvisited_children[0]
                else:
                    ucb_values = [
                        self.ucb_formula(
                            child.properties["reward"],
                            child.properties["visits"],
                            current_node.properties["visits"],
                            child.properties["sum_x_squared"]
                        ) for child in current_node.children
                    ]
                    child = current_node.children[ucb_values.index(max(ucb_values))]
                
                reward = self.rollout(problem, child)

                child.properties["reward"] += reward
                child.properties["visits"] += 1
                child.properties["sum_x_squared"] += reward**2

                current_node.properties["reward"] += reward
                current_node.properties["visits"] += 1
                current_node.properties["sum_x_squared"] += reward**2

            for i, child in enumerate(current_node.children):
                print(f"{i}. Action: {child.action} Reward: {child.properties['reward']} Visits: {child.properties['visits']}")
                
            selection_index = 0
            if selection_policy == "max_q_value_child":
                q_values = [child.properties["reward"] / child.properties["visits"] for child in current_node.children]
                selection_index = q_values.index(max(q_values))
            elif selection_policy == "max_child":
                rewards = [child.properties["reward"] for child in current_node.children]
                selection_index = rewards.index(max(rewards))
            elif selection_policy == "robust_child":
                visits = [child.properties["visits"] for child in current_node.children]
                selection_index = visits.index(max(visits))
            current_node = current_node.children[selection_index]

            current_node.properties["visits"] = 0
            problem.play_action(current_node.action)
            moves_played += 1
            print("Playing action number:", moves_played, "whose index is:", selection_index)
            problem.pprint()
        
        return {"success": True, "solution": current_node.get_actions(), "state_space_tree": root}

    def __str__(self):
        return "Flat Monte Carlo Solver"
