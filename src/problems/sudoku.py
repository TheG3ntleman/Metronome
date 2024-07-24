from src.primitives.problem import Problem
from random import sample
from numba import njit
import numpy as np
from math import sqrt

@njit
def sudoku_is_valid_move(board, i, j, value):
    side = len(board)
    base = int(sqrt(side))
    assert base == sqrt(side), "Board size must be a perfect square"

    # Check if the value is not in the row
    for column in range(side):
        if board[i][column] == value:
            return False

    # Check if the value is not in the column
    for row in range(side):
        if board[row][j] == value:
            return False

    # Check if the value is not in the 3x3 grid
    startRow, startCol = base * (i // base), base * (j // base)
    for row in range(startRow, startRow + base):
        for col in range(startCol, startCol + base):
            if board[row][col] == value:
                return False

    return True

@njit
def sudoku_get_valid_moves(board):
    side = len(board)
    assert int(sqrt(side)) == sqrt(side), "Board size must be a perfect square"

    valid_actions = []
    for i in range(side):
        for j in range(side):
            if board[i][j] == 0:
                for value in range(1, side + 1):
                    if sudoku_is_valid_move(board, i, j, value):
                        valid_actions.append((i, j, value))
    return valid_actions

@njit
def sudoku_get_valid_split_moves(board):
    # Moves are split by only finding the valid moves of a
    # particular empty cell (the one that occurs first
    # while scanning the board in row major order) and then
    # returning all the valid moves for that cell

    side = len(board)
    assert int(sqrt(side)) == sqrt(side), "Board size must be a perfect square"

    for i in range(side):
        for j in range(side):
            if board[i][j] == 0:
                valid_moves = []
                for value in range(1, side + 1):
                    if sudoku_is_valid_move(board, i, j, value):
                        valid_moves.append((i, j, value))
                return valid_moves

@njit
def sudoku_get_score(board):
    side = len(board)
    base = int(sqrt(side))
    assert base == sqrt(side), "Board size must be a perfect square"

    score = 0
    # Check rows and columns
    for i in range(side):
        row_set = set()
        col_set = set()
        for j in range(side):
            if board[i][j] != 0:
                row_set.add(board[i][j])
            if board[j][i] != 0:
                col_set.add(board[j][i])
        score += len(row_set) + len(col_set)

    # Check 3x3 subgrids
    for x in range(0, side, base):
        for y in range(0, side, base):
            subgrid_set = set()
            for i in range(base):
                for j in range(base):
                    if board[x+i][y+j] != 0:
                        subgrid_set.add(board[x+i][y+j])
            score += len(subgrid_set)
            
    # Penality for each empty cell
    for i in range(side):
        for j in range(side):
            if board[i][j] == 0:
                # Check if there are any valid moves for this cell
                if len([1 for value in range(1, side + 1) if sudoku_is_valid_move(board, i, j, value)]) == 0:
                    score -= 100
                else: 
                    score -= 1

    return score


def sudoku_generate_NxN(base, fraction_of_empties=0.5):
    side = base**2

    # pattern for a baseline valid solution
    def pattern(r, c):
        return (base * (r % base) + r // base + c) % side

    # randomize rows, columns and numbers (of valid base pattern)
    def shuffle(s):
        return sample(s, len(s))

    rBase = range(base)
    rows = [g * base + r for g in shuffle(rBase) for r in shuffle(rBase)]
    cols = [g * base + c for g in shuffle(rBase) for c in shuffle(rBase)]
    nums = shuffle(range(1, base * base + 1))

    # produce board using randomized baseline pattern
    board = [[nums[pattern(r, c)] for c in cols] for r in rows]

    squares = side * side
    empties = int(squares * fraction_of_empties)
    print(empties)
    for p in sample(range(squares), empties):
        board[p // side][p % side] = 0

    return np.asarray(board, dtype=np.int32)


class Sudoku(Problem):
    
    def __init__(self, base, board = None, fraction_of_empty_cells = 0.25, use_split_moves = False):
        self.board = board if board is not None else sudoku_generate_NxN(base, fraction_of_empties = fraction_of_empty_cells)
        self.base = base
        self.side = base**2
        self.use_split_moves = use_split_moves
    
    def clear(self):
        self.board = np.zeros((self.side, self.side), dtype=np.int32)
        
    def apply_action(self, action):
        i, j, value = action
        self.board[i][j] = value
    
    def get_valid_moves(self):
        return sudoku_get_valid_split_moves(self.board) if self.use_split_moves else sudoku_get_valid_moves(self.board)    

    def get_score(self):
        return sudoku_get_score(self.board)

    def get_penalty(self):
        if self.is_game_finished():
            return 10000
        return -100  # Penalty if the game is not finished but no moves are possible
    
    def is_game_finished(self):
        # Check if the board is filled
        for i in range(self.side):
            for j in range(self.side):
                if self.board[i][j] == 0:
                    return False

        return True

    def save_state(self):
        return [row[:] for row in self.board]  # Return a copy of the board

    def load_state(self, state):
        self.board = [row[:] for row in state]  # Load a copy of the state
    
    def pprint(self):
        for row in self.board:
            print(row)
        print("\n")
