"""
    In this file, we will implement the SolverProbeAnalysis class, which will be used to analyze the results of the NMCS, NRPA, GNRPA.
"""
from src.primitives.state_space_tree import StateSpaceTreeNode
from src.primitives.problem import Problem
from src.primitives.probe import Probe
from src.primitives.solver import Solver


import numpy as np # type: ignore
import matplotlib.pyplot as plt # type: ignore
import seaborn as sns # type: ignore
from sklearn.neighbors import KernelDensity # type: ignore
from scipy.stats import skew, kurtosis # type: ignore
import pandas as pd # type: ignore


class SolverAnalysis:
    
    def __init__(self, solvers: list[Solver], problem: Problem, number_of_probes = 100) -> None:
        self.solvers = solvers
        self.problem = problem
        self.number_of_probes = number_of_probes
        self.probes = []
        self.node = StateSpaceTreeNode(None)
        
        for solver in self.solvers:
            node = StateSpaceTreeNode(None)
            probe = Probe(node, solver, problem, number_of_probes)
            self.probes.append(probe)
    
        for move in self.problem.get_valid_moves():
            child = StateSpaceTreeNode(move, self.node)
            self.node.add_child(child)
            
    def perform_analysis(self):
        for probe in self.probes:
            probe.probe()

    def scatter_plot_rewards(self, save_path=None, show=False):
            colors = sns.color_palette("hsv", len(self.solvers))
            for i in range(self.node.get_number_of_children()):
                for idx, probe in enumerate(self.probes):
                    solver_name = type(self.solvers[idx]).__name__
                    plt.scatter(range(probe.number_of_probes), probe.reward_samples[i, :], label=f"{solver_name} Child {i}", color=colors[idx], alpha=0.6)
                plt.xlabel("Probe Number")
                plt.ylabel("Reward")
                plt.title(f"Reward Distribution for Child {i}")
                plt.legend()
                if save_path is not None:
                    plt.savefig(f"{save_path}combined_reward_distribution_{i}.eps", format="eps")
                if show:
                    plt.show()
                plt.clf()

    def kernel_density_plots(self, save_path=None, show=False):
        colors = sns.color_palette("hsv", len(self.solvers))
        for i in range(self.node.get_number_of_children()):
            # plt.figure(figsize=(10, 6))
            for idx, probe in enumerate(self.probes):
                solver_name = type(self.solvers[idx]).__name__
                important_list = probe.reward_samples[i, :]
                data = np.array(important_list).reshape(-1, 1)
                kde = KernelDensity(kernel='gaussian', bandwidth=0.75).fit(data)
                x_d = np.linspace(min(important_list) - 1, max(important_list) + 1, 1000).reshape(-1, 1)
                log_dens = kde.score_samples(x_d)

                plt.fill_between(x_d[:, 0], np.exp(log_dens), alpha=0.5, color=colors[idx], label=f"{solver_name} Child {i}")
                plt.plot(data[:, 0], -0.01 - 0.1 * np.random.rand(len(data)), 'ro', alpha=0.5)

            plt.title(f"Kernel Density Estimation for Child {i}")
            plt.xlabel("Reward")
            plt.ylabel("Density")
            plt.legend()
            if save_path is not None:
                plt.savefig(f"{save_path}combined_reward_density_{i}.png", format="png")
                
            if show == True:
                plt.show()
            plt.clf()

    def compare_statistics(self):
        stats = {'solver': [], 'child': [], 'mean': [], 'std_dev': [], 'skewness': [], 'kurtosis': []}
        for idx, probe in enumerate(self.probes):
            solver_name = type(self.solvers[idx]).__name__
            for i in range(probe.node.get_number_of_children()):
                stats['solver'].append(solver_name)
                stats['child'].append(i)
                stats['mean'].append(np.mean(probe.reward_samples[i, :]))
                stats['std_dev'].append(np.std(probe.reward_samples[i, :]))
                stats['skewness'].append(skew(probe.reward_samples[i, :]))
                stats['kurtosis'].append(kurtosis(probe.reward_samples[i, :]))

        df_stats = pd.DataFrame(stats)
        print("Statistics for the solvers: ", df_stats)
        return df_stats

    def save(self, save_path):
        for probe in self.probes:
            probe.save(save_path)

    
    