from src.primitives.solver import Solver
from src.primitives.state_space_tree import StateSpaceTreeNode
from tqdm import tqdm
import random

class FlatMonteCarloSolver(Solver):

    def __init__(self):
        super().__init__() 
    
    def initialize_node(self, node : StateSpaceTreeNode):
        node.properties["visits"] = 0
        node.properties["reward"] = 0  
        
    def select_node(self, node : StateSpaceTreeNode):
        return random.choice(node.children)

    def solve(self, problem, number_of_rollouts_per_decision = 1000, selection_policy = "max_q_value_child"):
        problem.pprint()
        # Initializing the state space tree
        root = StateSpaceTreeNode(None)
        current_node = root 
        actions_played = 0
        
        while not problem.is_game_finished():

            # STEP 1 : Getting the valid moves at this state
            valid_moves = problem.get_valid_moves()

            # Checking if there are any valid moves
            if len(valid_moves) == 0:
                # This means that we have failed to find a valid
                # solution. Since, FlatMC has no backtracking, this
                # becomes a failed run. However, instead of returning no 
                # solution we return what we ended up choosing at each step.
                return {"success": False, "solution": current_node.get_actions(), "state_space_tree": root}

            # STEP 3 : Expanding node
            for move in valid_moves:
                child = StateSpaceTreeNode(move, current_node)
                child.properties["visits"] = 0
                child.properties["reward"] = 0
                current_node.add_child(child)

            # STEP 4: Performing rollouts to make decision
            for _ in tqdm(range(number_of_rollouts_per_decision)):
                # Choosing a random child
                child = current_node.children[random.randint(0, len(current_node.children) - 1)]
                
                # Performing rollout
                reward = self.rollout(problem, child)

                child.properties["reward"] += reward
                child.properties["visits"] += 1

            # STEP 5: Choosing some policy by using methods proposed by Chaslot.
            
            # printing final rewarsd anf visits
            selection_index = 0
            if selection_policy == "max_q_value_child":
                print("Length of children: ", len(current_node.children))
                q_values = [child.properties["reward"] / child.properties["visits"] for child in current_node.children]
                current_node = current_node.children[q_values.index(max(q_values))]
                selection_index = q_values.index(max(q_values))
            elif selection_policy == "max_child":
                rewards = [child.properties["reward"] for child in current_node.children]
                current_node = current_node.children[rewards.index(max(rewards))]
                selection_index = rewards.index(max(rewards))
            elif selection_policy == "robust_child":
                visits = [child.properties["visits"] for child in current_node.children]
                current_node = current_node.children[visits.index(max(visits))]
                selection_index = visits.index(max(visits))
            
            # TODO: Add support for max_robust_child

            # STEP 6: Applying the action
            problem.play_action(current_node.action)
            actions_played += 1
            print("Playing action number:", actions_played, "whose index is:", selection_index)
            problem.pprint()
        
        return {"success": True, "solution": current_node.get_actions(), "state_space_tree": root}


    def __str__(self):
        return "Flat Monte Carlo Solver"