from src.primitives.state_space_tree import StateSpaceTreeNode
from src.primitives.problem import Problem
from src.primitives.solver import Solver
import numpy as np # type: ignore
import pandas as pd # type: ignore
import matplotlib.pyplot as plt # type: ignore
from tqdm import tqdm  # type: ignore
import seaborn as sns # type: ignore
import copy
import matplotlib.pyplot as plt # type: ignore
import warnings


"""# tower_of_hanoi_3x3 = TowerOfHanoi(3, 3)


# morpion_solitaire_5d = MorpionSolitaire5D(5)

1. We have no tree but we have a probe_lists problem. And we have a selection mechanism for sampling from that problem. Then the question is, should this be the main object on which the probe acts, i.e, should it act on the problem according to some selection policy.

0 - Probe

- This is very restrictive because we must analyze the entire problem.

2. Instead of acting on the problem, we act on a sub-problem. The reason we do this, is 
because a problem itself is a multi-step decision process. Meaning that we can potentially analyze the behaviour of a selection algorithm at a substep itself.

The problem is that we have a problem, and we have a solver. The solver is a function that takes in a problem and returns a solution. The solution is a sequence of actions that need to be taken to solve the problem. The solver can be a heuristic, or it can be a search algorithm. The solver can also be a reinforcement learning algorithm.   

        0
       / \
      /   \
     1      2
       \   / \
        3 4   5

- This is much more flexible and we can get more information here.
- Restriction: the probe can only act on leaf nodes.

We then adopt the second option.

"""


class Probe:
    def __init__(self, node : StateSpaceTreeNode, solver : Solver, problem : Problem, number_of_probes = 100):

        self.node = node
        self.solver = solver
        self.number_of_probes = number_of_probes
        
        self.problem = problem
        self.original_problem_state = self.problem.save_state()     

        # We first check if the node is a leaf node or not
        # we throw an assertion error if it isn't

        assert self.node.is_leaf(), "The node is not a leaf node."

        # We now need to get the children of the node
        # For problem, we will have to get the valid moves
        valid_moves = self.problem.get_valid_moves()    
        for move in valid_moves:
            child = StateSpaceTreeNode(move, parent=self.node)
            self.node.add_child(child)
            self.solver.initialize_node(child)

        # If the child has no nodes, we throw a warning
        warnings.warn("This node has no children.")

        # We now declare our data storage variables

        self.reward_samples = np.zeros((self.node.get_number_of_children(), self.number_of_probes), dtype = np.uint)
        
        self.mean_history = np.zeros((self.node.get_number_of_children(), self.number_of_probes))
        self.standard_deviation = np.zeros((self.node.get_number_of_children(), self.number_of_probes))
        self.skewness_history = np.zeros((self.node.get_number_of_children(), self.number_of_probes))
        self.kurtosis_history = np.zeros((self.node.get_number_of_children(), self.number_of_probes))


    def probe(self):
        for sample_number in tqdm(range(self.number_of_probes), desc="Probing"):
            for i, child in enumerate(self.node.children):
                # Loading original problem state into problem
                self.problem.load_state(self.original_problem_state)

                # Performing rollout to obtain reward
                reward = self.solver.rollout(self.problem, child)

                # Storing the reward and computing additional statistics
                self.reward_samples[i, sample_number] = reward
                self.compute_statistics(i, sample_number)

    def compute_statistics(self, child_number, sample_number):
        mean = np.mean(self.reward_samples[child_number, :sample_number])

        if sample_number > 1:
            std_dev = np.std(self.reward_samples[child_number, :sample_number])
            skewness = np.mean((self.reward_samples[child_number, :sample_number] - mean)**3) / (std_dev**3)
            kurtosis = np.mean((self.reward_samples[child_number, :sample_number] - mean)**4) / (std_dev**4) - 3
        else:
            std_dev = 0
            skewness = 0
            kurtosis = 0

        self.mean_history[child_number, sample_number] = mean
        self.standard_deviation[child_number, sample_number] = std_dev
        self.skewness_history[child_number, sample_number] = skewness
        self.kurtosis_history[child_number, sample_number] = kurtosis
    
    def scatter_plot_rewards(self, save_path = None):
        # TODO: this. -> Done
        print("Number of children: ", self.node.get_number_of_children())
        print("First Child children: ", self.node.children[0].children)
        for i in range(self.node.get_number_of_children()):
            plt.scatter(range(self.number_of_probes), self.reward_samples[i, :], label=f"Child {i}")
            plt.xlabel("Probe Number")
            plt.ylabel("Reward")
            plt.title(f"Reward Distribution for Child {i}")
            plt.legend()
            plt.savefig(f"reward_distribution_{i}.eps", format="eps")
            plt.show()

        """

        child_id = 0
        for _ in tqdm(range(self.number_of_probes), desc="Probing"):
            temporary_problem_copy = copy.deepcopy(self.problem)
            child = self.node.children[child_id]
            self.probe_lists[child_id].append(Solver.rollout(temporary_problem_copy, child))
            child_id += 1
            child_id = child_id % len(self.node.children)
            self.compute_statistics()

        dataframes = []
        for i in range(len(self.node.children)):
            dataframes.append(pd.DataFrame({
                "mean": self.mean_history[i],
                "skewness": self.skewness_history[i],
                "kurtosis": self.kurtosis_history[i]
            }))
        return dataframes """

"""

class Probe:
    def __init__(self, problem: Problem, solver: Solver, number_of_probes: int = 1000):
        self.problem = problem
        self.number_of_probes = number_of_probes
        # For node, we will have to get the children of the node
        # For problem, we will have to get the valid moves
        root = StateSpaceTreeNode(None)
        valid_moves = problem.get_valid_moves()
        for move in valid_moves:
            child = StateSpaceTreeNode(move, parent=root)
            root.add_child(child)
            child.properties["visits"], child.properties["reward"] = (0,0)

        self.node = root
        self.probe_lists = [[] for _ in range(len(self.node.children))]
        self.mean_history = [[] for _ in range(len(self.node.children))]
        self.skewness_history = [[] for _ in range(len(self.node.children))]
        self.kurtosis_history = [[] for _ in range(len(self.node.children))]
    
    def probe(self):
        if not self.node.children:
            raise ValueError("The node has no children to probe.")


        child_id = 0
        for _ in tqdm(range(self.number_of_probes), desc="Probing"):
            temporary_problem_copy = copy.deepcopy(self.problem)
            child = self.node.children[child_id]
            self.probe_lists[child_id].append(Solver.rollout(temporary_problem_copy, child))
            child_id += 1
            child_id = child_id % len(self.node.children)
            self.compute_statistics()

        dataframes = []
        for i in range(len(self.node.children)):
            dataframes.append(pd.DataFrame({
                "mean": self.mean_history[i],
                "skewness": self.skewness_history[i],
                "kurtosis": self.kurtosis_history[i]
            }))
        return dataframes 

        # We need to create an instance of the solver
        self.solver = solver()
        
        # TODO: We need to get the rewards of the children, after the solver has been run -> Need to implement this
        
        # Assuming that the solver has been run, we will now probe the children
        
        # self.child_rewards = [[] for _ in range(len(self.node.children))], not exactly self.node.children but for time pass

        # self.child_at_every_step = [], same size as above, except it keeps track of the child that was chosen at every step
        
        self.child_rewards, self.child_at_every_step = self.solver.solve(self.problem, number_of_probes)
        
        
    def probe(self):
        if not self.node.children:
            raise ValueError("The node has no children to probe.")


        child_id = 0
        for _ in tqdm(range(self.number_of_probes), desc="Probing"):
            temporary_problem_copy = copy.deepcopy(self.problem)
            child = self.node.children[child_id]
            self.probe_lists[child_id].append(Solver.rollout(temporary_problem_copy, child))
            child_id += 1
            child_id = child_id % len(self.node.children)
            self.compute_statistics()

        dataframes = []
        for i in range(len(self.node.children)):
            dataframes.append(pd.DataFrame({
                "mean": self.mean_history[i],
                "skewness": self.skewness_history[i],
                "kurtosis": self.kurtosis_history[i]
            }))
        return dataframes    


    def compute_statistics(self):
        for i in range(len(self.node.children)):
            if len(self.probe_lists[i]) > 0: 
                mean = np.mean(self.probe_lists[i])
                skewness = np.mean((self.probe_lists[i] - mean)**3) / (np.std(self.probe_lists[i])**3)
                kurtosis = np.mean((self.probe_lists[i] - mean)**4) / (np.var(self.probe_lists[i])**2) - 3
                
                self.mean_history[i].append(mean)
                self.skewness_history[i].append(skewness)
                self.kurtosis_history[i].append(kurtosis)
            

    def get_statistics(self):
        return self.mean_history, self.skewness_history, self.kurtosis_history

    
    def get_distributions_for_each_sample(self, i: any):
        if i is not None:
            return self.probe_lists[i]
        return self.probe_lists

        
    def scatter_plot_rewards(self):
        for i in range(len(self.node.children)):
            plt.scatter(range(len(self.probe_lists[i])), self.probe_lists[i], label=f"Child {i}")
            plt.xlabel("Probe Number")
            plt.ylabel("Reward")
            plt.title(f"Reward Distribution for Child {i}")
            plt.legend()
            plt.savefig(f"reward_distribution_{i}.eps", format="eps")
            plt.show()

    def show_child_distribution(self):
        for i in range(len(self.child_rewards)):
            for j, reward in enumerate(self.child_rewards[i]):
                if j in self.child_at_every_step:
                    # Plot in red, if the child was chosen
                    plt.scatter(j, reward, color='red')
                else:
                    # Plot normally
                    plt.scatter(j, reward, color='blue')
            
            plt.xlabel("Probe Number")
            plt.ylabel("Reward")
            plt.title(f"Reward Distribution for Child {i}")
            plt.legend()
            plt.savefig(f"reward_distribution_{i}.eps", format="eps")
            plt.show()                    
    
    
    def show_final_distributions(self):

        max_length = max(len(lst) for lst in self.probe_lists)
        probe_lists_padded = [lst + [np.nan]*(max_length - len(lst)) for lst in self.probe_lists]
        probe_lists_array = np.array(probe_lists_padded)
        sns.heatmap(probe_lists_array, annot=False, cmap='viridis', yticklabels=[f'probe_list {i+1}' for i in range(len(self.probe_lists))])
        plt.xlabel('Probe Number')
        plt.ylabel('Probe List')
        plt.show()"""