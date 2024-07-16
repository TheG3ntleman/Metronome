"""
from src.primitives.state_space_tree import StateSpaceTreeNode
from src.primitives.problem import Problem
from src.primitives.solver import Solver
import copy
import pandas as pd
from tqdm import tqdm

class LegacyProbe:

    @staticmethod
    def probe(node : StateSpaceTreeNode, problem : Problem, number_of_probes = 1000):
        # Setting up a list for each child
        probe_lists = [[] for _ in range(len(node.children))]

        mean_history = [[] for _ in range(len(node.children))]
        variance_history = [[] for _ in range(len(node.children))]
        skewness_history = [[] for _ in range(len(node.children))]
        kurtosis_history = [[] for _ in range(len(node.children))]                                                       

        # Performing the probes
        child_id = 0
        for _ in tqdm(range(number_of_probes)):

            temporary_problem_copy = copy.deepcopy(problem)

            # Getting the child
            child = node.children[child_id]

            # Getting a reward from a flat rollout
            probe_lists[child_id].append(Solver.rollout(temporary_problem_copy, child))

            # Child id rotates in a round robin fashion
            child_id += 1
            child_id = child_id % len(node.children)

            # We now iterate through all child lists, if they are non-empty
            # then we calculate the mean, variance, skewness and kurtosis
            for i in range(len(node.children)):
                if len(probe_lists[i]) > 0:
                    mean_history[i].append(sum(probe_lists[i]) / len(probe_lists[i]))
                    variance_history[i].append(sum([(x - mean_history[i][-1]) ** 2 for x in probe_lists[i]]) / len(probe_lists[i]))
                    skewness_history[i].append(sum([(x - mean_history[i][-1]) ** 3 for x in probe_lists[i]]) / len(probe_lists[i]))
                    kurtosis_history[i].append(sum([(x - mean_history[i][-1]) ** 4 for x in probe_lists[i]]) / len(probe_lists[i]))

        # we now create a pandas dataframe with the mean, variance, skewness and kurtosis for each child and return a list of these dataframes

        dataframes = []

        for i in range(len(node.children)):
            dataframes.append(pd.DataFrame({"mean": mean_history[i], "variance": variance_history[i], "skewness": skewness_history[i], "kurtosis": kurtosis_history[i]}))
        
        return dataframes
"""

from src.primitives.state_space_tree import StateSpaceTreeNode
from src.primitives.problem import Problem
from src.primitives.solver import Solver
import copy
import pandas as pd
from tqdm import tqdm
import numpy as np

from matplotlib import pyplot as plt

class LegacyProbe:

    @staticmethod
    def probe(node: StateSpaceTreeNode, problem: Problem, number_of_probes=1000):
        if not node.children:
            raise ValueError("The node has no children to probe.")

        # Setting up a list for each child
        probe_lists = [[] for _ in range(len(node.children))]

        mean_history = [[] for _ in range(len(node.children))]
        variance_history = [[] for _ in range(len(node.children))]
        skewness_history = [[] for _ in range(len(node.children))]
        kurtosis_history = [[] for _ in range(len(node.children))]                                                       

        # Performing the probes
        child_id = 0
        for _ in tqdm(range(number_of_probes), desc="Probing"):

            temporary_problem_copy = copy.deepcopy(problem)

            # Getting the child
            child = node.children[child_id]

            # Getting a reward from a flat rollout
            probe_lists[child_id].append(Solver.rollout(temporary_problem_copy, child))

            # Child id rotates in a round robin fashion
            child_id += 1
            child_id = child_id % len(node.children)

            # We now iterate through all child lists, if they are non-empty
            # then we calculate the mean, variance, skewness and kurtosis
            for i in range(len(node.children)):
                if len(probe_lists[i]) > 0:
                    mean_history[i].append(np.mean(probe_lists[i]))
                    variance_history[i].append(np.var(probe_lists[i], ddof=1))  # Sample variance
                    skewness_history[i].append(np.mean(((probe_lists[i] - mean_history[i][-1]) ** 3)) / (np.var(probe_lists[i], ddof=1) ** 1.5))  # Skewness
                    kurtosis_history[i].append(np.mean(((probe_lists[i] - mean_history[i][-1]) ** 4)) / (np.var(probe_lists[i], ddof=1) ** 2) - 3)  # Kurtosis

        # We now create a pandas dataframe with the mean, variance, skewness and kurtosis for each child and return a list of these dataframes
        dataframes = []

        for i in range(len(node.children)):
            dataframes.append(pd.DataFrame({
                "mean": mean_history[i],
                "variance": variance_history[i],
                "skewness": skewness_history[i],
                "kurtosis": kurtosis_history[i]
            }))
        
        # We iterate through the children and produce a scatterplot of the 
        # distribution of rewards for each child
        for i in range(len(node.children)):
            plt.scatter(range(len(probe_lists[i])), probe_lists[i], label=f"Child {i}")
            plt.xlabel("Probe Number")
            plt.ylabel("Reward")
            plt.title(f"Reward Distribution for Child {i}")
            plt.legend()
            plt.savefig(f"reward_distribution_{i}.eps", format="eps")
            plt.show()

        return dataframes
