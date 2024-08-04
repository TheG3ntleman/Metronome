import copy
import random
import numpy as np  # type: ignore
from tqdm import tqdm  # type: ignore
import math
from collections import Counter
import matplotlib.pyplot as plt  # type: ignore
from src.primitives.solver import Solver
from src.primitives.problem import Problem


class NRPA(Solver):
    def __init__(self):
        super().__init__()

    def solve(self, problem, max_level=3, max_iterations=1000):
        self.problem = problem
        self.max_level = max_level
        self.max_iterations = max_iterations
        self.scores = []
        self.best_sequence = []

        def policy(level, problem, policy):
            return policy

        def adapt_policy(policy, sequence):
            adapted_policy = policy.copy()
            # Here, you can implement your logic to adapt the policy based on the sequence
            return adapted_policy

        def sample(problem):
            """
            Perform a single random rollout from the given position and return the score.
            """
            problem_copy = copy.deepcopy(problem)
            while not problem_copy.is_game_finished():
                valid_moves = problem_copy.get_valid_moves()
                if not valid_moves:
                    return problem_copy.get_penalty() + problem_copy.get_score()
                move = random.choice(valid_moves)
                problem_copy.apply_action(move)
            return problem_copy.get_score()

        def nested(problem, level, policy):
            """
            Perform a nested Monte Carlo search up to the given level.
            """
            best_score = -float("inf")
            best_sequence = []

            while not problem.is_game_finished():
                if level == 1:
                    # Perform a level-1 search using the sample function
                    move = max(
                        problem.get_valid_moves(),
                        key=lambda m: sample(play_action(problem, m)),
                    )
                else:
                    # Perform a recursive nested search at the next level
                    move = max(
                        problem.get_valid_moves(),
                        key=lambda m: nested(
                            play_action(problem, m), level - 1, policy
                        )[0],
                    )

                # Apply the move and update the score if it's the best
                problem.apply_action(move)
                current_score = problem.get_score()

                if current_score > best_score:
                    best_score = current_score
                    best_sequence = [move] + best_sequence

            return best_score, best_sequence

        def iterative_nested(problem):
            """
            Perform the NMCS iteratively with a fixed number of iterations.
            """
            best_score = -float("inf")
            best_sequence = []

            # Initialize policy based on the number of valid moves
            initial_moves = problem.get_valid_moves()
            policy_size = len(initial_moves)
            policy = np.zeros(policy_size)  # Initialize policy with zeros

            for _ in tqdm(range(self.max_iterations), desc="Iterations"):
                sequence = []
                score, sequence = nested(problem, self.max_level, policy)
                policy = adapt_policy(policy, sequence)
                self.scores.append(score)  # Track scores for plotting
                if score > best_score:
                    best_score = score
                    best_sequence = sequence

            return best_score, best_sequence

        def play_action(problem, action):
            """
            Helper function to create a new problem state with the action applied.
            """
            problem_copy = copy.deepcopy(problem)
            problem_copy.apply_action(action)
            return problem_copy

        # Perform iterative nested search to find the best score and sequence
        best_score, best_sequence = iterative_nested(problem)

        # This method should return a dictionary as specified in the Solver class.
        # In a real implementation, you would also include the state space tree.
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
