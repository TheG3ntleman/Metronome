from src.primitives.solver import Solver
from src.primitives.state_space_tree import StateSpaceTreeNode
from tqdm import tqdm
import random
import math

class UCB1Solver(Solver):

    def __init__(self, exploration_parameter=1 / math.sqrt(2)):
        super().__init__()
        self.exploration_parameter = exploration_parameter
        self.ucb_formula = lambda r, n, N: (
            r / n
        ) + self.exploration_parameter * math.sqrt(math.log(N) / n)
    
    def solve(self, problem, number_of_rollouts_per_decision = 1000, selection_policy = "max_q_value_child"):
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
                #child = current_node.children[random.randint(0, len(current_node.children) - 1)]

                # Checking if there is a child that has not been visited
                unvisited_children = [child for child in current_node.children if child.properties["visits"] == 0]
                if len(unvisited_children) > 0:
                    child = unvisited_children[0]
                else:
                    # Choosing the child with the highest UCB1 value
                    ucb_values = [self.ucb_formula(child.properties["reward"], child.properties["visits"], current_node.properties["visits"]) for child in current_node.children]
                    child = current_node.children[ucb_values.index(max(ucb_values))]
                
                # Performing rollout
                reward = self.rollout(problem, child)

                child.properties["reward"] += reward
                child.properties["visits"] += 1

                # Cheap backpropagation
                current_node.properties["reward"] += reward
                current_node.properties["visits"] += 1

            # STEP 5: Choosing some policy by using methods proposed by Chaslot.

            for i, child in enumerate(current_node.children):
                print(f"{i}. Action: {child.action} Reward: {child.properties['reward']} Visits: {child.properties['visits']}")
                
            # if selection_policy == "max_q_value_child":
            #     q_values = [child.properties["reward"] / child.properties["visits"] for child in current_node.children]
            #     current_node = current_node.children[q_values.index(max(q_values))]
            # elif selection_policy == "max_child":
            #     rewards = [child.properties["reward"] for child in current_node.children]
            #     current_node = current_node.children[rewards.index(max(rewards))]
            # elif selection_policy == "robust_child":
            #     visits = [child.properties["visits"] for child in current_node.children]
            #     current_node = current_node.children[visits.index(max(visits))]

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
            
            # TODO: Add support for max_robust_child

            # STEP 6: Applying the action
            current_node.properties["visits"] = 0
            problem.play_action(current_node.action)
            moves_played += 1
            print("Playing action number:", moves_played, "whose index is:", selection_index)
            problem.pprint()
        
        return {"success": True, "solution": current_node.get_actions(), "state_space_tree": root}


    def __str__(self):
        return "Flat Monte Carlo Solver"