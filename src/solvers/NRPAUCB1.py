import numpy as np  # type: ignore
from tqdm import tqdm  # type: ignore
import random
import math
from collections import defaultdict, Counter
import matplotlib.pyplot as plt  # type: ignore
from src.primitives.solver import Solver
from src.primitives.problem import Problem
from copy import deepcopy

class NRPAUCB(Solver):
    def __init__(self):
        super().__init__()

    def solve(self, problem, max_level=2, max_iterations=1, C=np.sqrt(2)):
        self.problem = problem
        self.max_level = max_level
        self.max_iterations = max_iterations
        self.C = C
        self.scores = []
        self.best_sequence = []

        def ucb1_action_selection(valid_moves, action_stats, total_visits):
            """
            Select an action using the UCB1 formula.
            """
            max_ucb_value = -float('inf')
            selected_action = None
            for action in valid_moves:
                score_sum, action_count = action_stats[action]
                if action_count == 0:
                    return action  # Choose unvisited actions first for exploration
                average_score = score_sum / action_count
                ucb_value = average_score + self.C * np.sqrt(np.log(total_visits) / action_count)
                if ucb_value > max_ucb_value:
                    max_ucb_value = ucb_value
                    selected_action = action
            return selected_action

        def sample(problem, action_stats, total_visits):
            """
            Perform a rollout using UCB1 to select actions.
            """
            problem_copy = deepcopy(problem)
            sequence = []
            total_score = 0

            while not problem_copy.is_game_finished():
                valid_moves = problem_copy.get_valid_moves()
                if not valid_moves:
                    return total_score + problem_copy.get_penalty(), sequence

                # Select action using UCB1
                action = ucb1_action_selection(valid_moves, action_stats, total_visits)
                problem_copy.apply_action(action)
                sequence.append(action)
                total_score = problem_copy.get_score()

            return total_score, sequence

        def update_statistics(action_stats, sequence, reward):
            """
            Update action statistics based on the reward and sequence.
            """
            for action in sequence:
                score_sum, action_count = action_stats[action]
                action_stats[action] = (score_sum + reward, action_count + 1)

        def nested(problem, level, action_stats, total_visits):
            """
            Perform a nested Monte Carlo search up to the given level.
            """
            if level == 1:
                score, sequence = sample(problem, action_stats, total_visits)
                update_statistics(action_stats, sequence, score)
                return score, sequence

            best_score = -float('inf')
            best_sequence = []

            while not problem.is_game_finished():
                score, sequence = nested(problem, level - 1, action_stats, total_visits)
                if score > best_score:
                    best_score = score
                    best_sequence = sequence

            update_statistics(action_stats, best_sequence, best_score)
            return best_score, best_sequence

        def iterative_nested(problem):
            """
            Perform the NMCS iteratively with a fixed number of iterations.
            """
            best_score = -float("inf")
            best_sequence = []

            action_stats = defaultdict(lambda: (0.0, 0))  # (score_sum, action_count)
            total_visits = 0

            for _ in tqdm(range(self.max_iterations), desc="Iterations"):
                total_visits += 1
                score, sequence = nested(problem, self.max_level, action_stats, total_visits)
                self.scores.append(score)  # Track scores for plotting
                if score > best_score:
                    best_score = score
                    best_sequence = sequence

            return best_score, best_sequence

        # Perform iterative nested search to find the best score and sequence
        best_score, best_sequence = iterative_nested(problem)

        return {
            "success": best_score > -float("inf"),
            "solution": best_score,
            "best_sequence": best_sequence,
            "state_space_tree": None,  # Here you would include the root of the state space tree if applicable
        }

    def plot_scores(self):
        """
        Plot the distribution of scores obtained during the search.
        """
        if not self.scores:
            print("No scores to plot.")
            return

        # Filter out invalid scores
        valid_scores = [
            score for score in self.scores if not (math.isnan(score) or math.isinf(score))
        ]
        if not valid_scores:
            print("No valid scores to plot.")
            return

        # Count the frequency of each score
        score_counts = Counter(valid_scores)
        scores = list(score_counts.keys())
        frequencies = list(score_counts.values())

        # Calculate probabilities as normalized frequencies
        total = sum(frequencies)
        probabilities = [freq / total for freq in frequencies]

        # Plotting
        plt.figure(figsize=(10, 6))
        plt.bar(scores, probabilities, width=0.4, color="skyblue", edgecolor="black")
        plt.xlabel("Score")
        plt.ylabel("Probability")
        plt.title("Score Distribution in NMCS")
        plt.grid(axis="y", linestyle="--", alpha=0.7)
        plt.xticks(scores)  # Ensure all scores are labeled on x-axis
        plt.show()
