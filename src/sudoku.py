import random
import numpy as np

class SudokuRules:
    def __init__(self, grid, size):
        self.grid = grid
        self.size = size
        
    def hard_is_row_valid(self, row, number):
        if (number in self.grid[row]):
            return False
        return True
        
    def hard_is_column_valid(self, col, number):
        if (number in self.grid[:, col]):
            return False
        return True
   
    def hard_is_subgrid_valid(self, row, col, number):
        size = self.size
        
        start_row = row - row % size
        start_col = col - col % size
        
        subgrid = self.grid[start_row:start_row+size, start_col:start_col+size]       
        return not np.any(subgrid == number)

    def is_valid(self, row, col, number):
        return self.hard_is_row_valid(row, number) and self.hard_is_column_valid(col, number) and self.hard_is_subgrid_valid(row, col, number)

class Sudoku:
    """
    Sudoku puzzle base class
    """

    def __init__(self, size):
        # Storing size related information
        self.size = size
        self.puzzle_size = size * size
        self.grid = np.zeros((self.puzzle_size, self.puzzle_size), dtype=int)
        
        # Making an instance of sudoku rules.
        self.rules = SudokuRules(self.grid, self.size)
    
    def print_puzzle(self):
        print("\nSudoku Puzzle of size", self.size)
        for i in range(self.puzzle_size):
            if i % self.size == 0 and i != 0:
                print("=" * int(self.puzzle_size * 4.1 + self.size + 1))
            for j in range(self.puzzle_size):
                if j % self.size == 0 and j != 0:
                    print("|", end=" ")
                print("{:3}".format(self.grid[i][j]), end=" ")
            print()
    

class SudokuFeasible(Sudoku):
    """
    All puzzles made in this class are guaranteed to have at least one solution.
    """

    def __init__(self, size):
        super().__init__(size)


class SudokuNoGuarantee(Sudoku):
    """
    No guarantee that the puzzle will have a solution.
    """

    def __init__(self, size):
        super().__init__(size)
        self.make_puzzle()

    def make_puzzle(self, prefill_percentage = 0.33):
      tosses = np.random.rand(self.puzzle_size, self.puzzle_size)
      for i in range(self.puzzle_size):
        for j in range(self.puzzle_size):
          if (tosses[i][j] < prefill_percentage):
            # Attempt to insert a non-zero value
            # that satisfies all the constraints:
            # 1. No repeated values in the row
            # 2. No repeated values in the column
            # 3. No repeated values in the subgrid
            # If no such value is found then repeat 
            # the entire function.
            
            # Listing all options according to the size of the puzzle.
            options = [i + 1 for i in range(self.puzzle_size)]
            for _ in range(self.puzzle_size):
                # Selecting an option
                selected_option = random.choice(options)
                
                # Checking if this option is a valid choice
                if self.rules.is_valid(i, j, selected_option):
                    # Adding th selected option to the grid
                    self.grid[i][j] = selected_option
                    break
                
                # Removing the selected option from the list of options
                options.remove(selected_option)
    
    
                
                
                
            
