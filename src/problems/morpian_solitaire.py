# Infinite grid is difficult to implement.
# Nodes with up, down, left, right, up-left, ...
# fields and references would be the best way
# to make an infinite grid.

# The difficulty lies in finding feasible moves,
# and updating feilds when needed as that would
# probably require the implementation of a neighbor
# search mechanism which would most likely be a
# pure pain.

# Another way to do it is to maintain a 2D array
# and resize it whenever necessary.

"""
getActionSize
getValidMoves
getGameEnded
%getGameStringRepresentation
getScore % Probably only works on terminal states.
getPenalty % When the game is not over, but the player can't take
            % any more moves.
"""

from src.primitives.problem import Problem
from src.problems.morpian_solitaire_logic import Board, Line, Move
from matplotlib import pyplot as plt
import numpy as np

class MorpionSolitaire5D(Problem):
    
    def __init__(self, board_size):
        self.board_size = board_size
        self.board = Board(board_size, True)
        
    def plot(self):
        # Assuming self.board and Board are already defined and available

        n = self.board.pieces.shape[0]
        b = Board(n)
        b.performedmoves = self.board.performedmoves.copy()

        initdot_x = []
        initdot_y = []
        for x in range(n):
            for y in range(n):
                if b[x][y] == 1:
                    initdot_x.append(x)
                    initdot_y.append(y)

        plt.grid(linestyle='--', linewidth=0.5)
        plt.xticks(np.arange(0, n, step=1))
        plt.yticks(np.arange(0, n, step=1))
        plt.scatter(initdot_x, initdot_y)

        for move in self.board.performedmoves:
            movedots_x = [move.point[0]]
            movedots_y = [move.point[1]]
            plt.scatter(movedots_x, movedots_y, color='none', marker='o', edgecolor='g')

            theline_x = [move.line.p1[0], move.line.p2[0]]
            theline_y = [move.line.p1[1], move.line.p2[1]]
            plt.plot(theline_x, theline_y, "r")
            #plt.pause(0.1)

        plt.show()


    # Implementing functions that must be overridden from the
    # problem class.
    def clear(self):
        self.board = Board(self.board_size, True)
    
    def apply_action(self, action):
        self.board = self.board.playMove(action)
    
    def get_valid_moves(self):
        return self.board.getPossibleMoves()

    def get_score(self):
        return self.board.getScore()
    
    def is_game_finished(self):
        if len(self.board.getPossibleMoves()) == 0:
            return True
        return False
    
    def get_game_state(self):
        return self.board
    
    def load_game_from_state(self, state):
        self.board = state

    def pprint(self):
        print("Can't print the board in morpion solitaire. Using plot() instead.")
        self.plot()
