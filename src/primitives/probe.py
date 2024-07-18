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
import pickle
from sklearn.neighbors import KernelDensity


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

        self.reward_samples = np.zeros((self.node.get_number_of_children(), self.number_of_probes), dtype = np.float64)
        
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
    
    def scatter_plot_rewards(self, save_path = None, show = False):
        # TODO: this. -> Done
        print("Number of children: ", self.node.get_number_of_children())
        print("First Child children: ", self.node.children[0].children)
        for i in range(self.node.get_number_of_children()):
            plt.scatter(range(self.number_of_probes), self.reward_samples[i, :], label=f"Child {i}")
            plt.xlabel("Probe Number")
            plt.ylabel("Reward")
            plt.title(f"Reward Distribution for Child {i}")
            plt.legend()
            if save_path is not None:
                plt.savefig(f"{save_path}reward_distribution_{i}.eps", format="eps")
            if show:
                plt.show()
            plt.clf()

    def kernel_density_plots(self, save_path = None, show = False):
        
        def plot_kde(important_list, bandwidth=0.75, kernel='gaussian', 
             title='Kernel Density Estimation', xlabel='Reward', ylabel='Density', color='dodgerblue'):
            """
            Plots the kernel density estimation for a given list of integers.
            
            Parameters:
                important_list (list): List of integer values to perform KDE on.
                bandwidth (float): The bandwidth of the kernel. Default is 0.75.
                kernel (str): The kernel to use for KDE. Default is 'gaussian'.
                            Options include 'gaussian', 'tophat', 'epanechnikov', 
                            'exponential', 'linear', 'cosine'.
                title (str): Title of the plot. Default is 'Kernel Density Estimation'.
                xlabel PUCT(str): Label for the x-axis. Default is 'Value'.
                ylabel (str): Label for the y-axis. Default is 'Density'.svg
                color (str): Color of the KDE plot. Default is 'blue'.
            """
            # Convert the list to a numpy array and reshape for scikit-learn
            data = np.array(important_list).reshape(-1, 1)

            # Create and fit the KDE model
            kde = KernelDensity(kernel=kernel, bandwidth=bandwidth).fit(data)

            # Create a range of values for the x-axis
            x_d = np.linspace(min(important_list) - 1, max(important_list) + 1, 1000).reshape(-1, 1)

            # Evaluate the KDE on the range of values
            log_dens = kde.score_samples(x_d)

            # Plot the results
            plt.fill_between(x_d[:, 0], np.exp(log_dens), color=color, alpha=0.5)
            plt.plot(data[:, 0], -0.01 - 0.1 * np.random.rand(len(data)), 'ro', label='Data points')
            plt.title(title)
            plt.xlabel(xlabel)
            plt.ylabel(ylabel)
            #plt.legend()
            if save_path is not None:
                plt.savefig(f"{save_path}reward_density_plots{i}.eps", format="eps")
            if show:
                plt.show()
            plt.clf()

        for i in range(self.node.get_number_of_children()):
            plot_kde(self.reward_samples[i, :], title=f"Kernel Density Estimation for Child {i}")

    def save(self, save_path):
        # Saving the rewardssave_path = None
        with open(f"{save_path}rewards.pkl", "wb") as f:
            pickle.dump(self.reward_samples, f)

    def load(self, save_path):
        with open(f"{save_path}rewards.pkl", "rb") as f:
            self.reward_samples = pickle.load(f)

        # TODO: Compute means, variances, skewnesses, kurtosis, ...
