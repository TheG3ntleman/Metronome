from src.primitives.solver import Solver
from src.primitives.state_space_tree import StateSpaceTreeNode
import random
from tqdm import tqdm # type: ignore

class FlatMonteCarloAgain(Solver):
    def __init__(self):
        super().__init__()
    
    def solve(self, problem, number_of_rollouts_per_decision = 1000, selection_policy = "max_q_value_child"):
        problem.pprint()
        # So, there will be 4 steps in this algorithm:
            # 1. Selection -> Choosing the best child, based on a selection policy.
            # 2. Expansion -> After reaching a leaf node, we expand it (based of it's actions)
            # 3. Simulation/Rollout -> We simulate the game from the expanded node.(A bunch of times)
            # 4. Backpropagation -> We update the values of the nodes in the path from the root to the expanded node.
        
        # Initializing the state space tree
        root = StateSpaceTreeNode(None)
        current_node = root
        actions_played = 0
        
        while problem.is_game_finished() == False:
            
            # First, find out the valid moves at this state
            valid_moves = problem.get_valid_moves()
            
            # If there are no valid moves, then we have failed to find a solution.
            if valid_moves == []:
                return {"success": False, "solution": current_node.get_actions(), "state_space_tree": root}
            
            
            # Expanding the node
            for move in valid_moves:
                child = StateSpaceTreeNode(move, current_node)
                child.properties["visits"] = 0
                child.properties["reward"] = 0
                current_node.add_child(child)
                
            if len(current_node.children) == 1:
                best_child = current_node.children[0]
                problem.play_action(best_child.action)
                problem.pprint()
                current_node = best_child
                continue
                
            # Performing rollouts to make decision
            for _ in tqdm(range(number_of_rollouts_per_decision)):
                # Choosing a random child
                child = current_node.children[random.randint(0, len(current_node.children) - 1)]
                
                # Performing rollout
                reward = self.rollout(problem, child)
                
                child.properties["reward"] += reward
                child.properties["visits"] += 1
                
            # and now, we choose the node with the best reward/visit ratio
            best_child: object = current_node.children[0]
            best_ratio = child.properties["reward"] / child.properties["visits"]
            
            print("Length of children: ", len(current_node.children))
            
            for child in current_node.children:
                # print(child.action)
                if child.properties["visits"] == 0:
                    continue
                ratio = child.properties["reward"] / child.properties["visits"]
                print("Ratio: ", ratio)
                if best_ratio < ratio:
                    print("HIIII")
                    best_ratio = max(ratio, best_ratio)
                    best_child = child
            
            print("Best child: ", best_child.action)
            print("Reward: ", best_child.properties["reward"])
            # Now, we move to the best child
            
            problem.play_action(best_child.action)
            
            actions_played+=1
            
            problem.pprint()
            
            current_node = best_child
            print("Actions played: ", actions_played)
            
        return {"success": True, "solution": current_node.get_actions(), "state_space_tree": root}
            
    
    def __str__(self):
        return "Flat Monte Carlo Again"