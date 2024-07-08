from mcts.primitives.problem import Problem
import numpy as np


class SameGame(Problem):
    def __init__(self, board_size, number_of_colors):
        self.board_size = board_size
        self.number_of_colors = number_of_colors

        # Initializing board
        self.board = np.random.randint(
            low=1, high=self.number_of_colors, size=[self.board_size, self.board_size]
        )
