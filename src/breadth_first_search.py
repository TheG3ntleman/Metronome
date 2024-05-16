from src.node import Node
from collections import deque
from copy import deepcopy
from src.sudoku import Sudoku, SudokuRules

class SudokuSolverBFS:
    
    def __init__(self, puzzle):
        self.puzzle = puzzle
        self.root_node = Node(0, 0, 0, None)

        
    def run(self, number_of_expansions = 50):
        
        # Some helper functions and variables, main bfs starts below
        number_of_possible_actions = self.puzzle.puzzle_size
        
        def apply_branch_to_grid(node, grid):
            current_node = node
            while current_node.parent is not None: # If the current node is not root
                grid[current_node.row][current_node.col] = current_node.action
                current_node = current_node.parent
            return grid
        
        # We first clarify the problem
        # by finding all the possible cells where
        # placement is possible, these are essentially the
        # row and column values of zero-cells
        
        points_of_actions = []
        for i in range(self.puzzle.puzzle_size):
            for j in range(self.puzzle.puzzle_size):
                if self.puzzle.grid[i][j] == 0:
                    points_of_actions.append((i, j))
                    
        #print("Points of action", points_of_actions)
        
        # STEP - 0: Initialize the queue and other structures
        queue = deque([self.root_node])
        
        # STEP - 1: Start the BFS
        for _ in range(number_of_expansions): # Run as long as computational budget is not exceeded
            
            # STEP - 1:0 : Check if the queue is empty (TERMINATION CRITERIA)
            if len(queue) == 0:
                break
            
            # STEP - 1:1 : Pop the first node from the queue
            current_node = queue.popleft()
            
            # STEP - 1:2 : Checking if the node is a leaf node 
            
            # Feasible leaf node implies solution (ANOTHER TERMINATION CRITERIA)
            if (current_node.depth == len(points_of_actions)):
                # A Solution has been found.
                return apply_branch_to_grid(current_node, deepcopy(self.puzzle.grid))
            
            # Continue if infeasible
            if not current_node.feasible:
                continue

            # STEP - 1:3 : Expand the current node for each action (also mark feasibility)
            # Getting row and column of grid on which the child nodes will perform an action
            row, col = points_of_actions[current_node.depth]
            
            for action in [i +1 for i in range(number_of_possible_actions)]:
                # Add child to the current node
                current_node.add_child(action, row, col, number_of_possible_actions)
                
                # Check if the action is feasible
                grid_of_actions_till_parent = apply_branch_to_grid(current_node, deepcopy(self.puzzle.grid))
                print(f"Trying action at row: {row} and col: {col} with action: {action}")
                print(f"Grid is:\n", grid_of_actions_till_parent)
                
                if not SudokuRules.is_valid(grid_of_actions_till_parent, self.puzzle.size, row, col, action):
                    print("Infeasible action")
                    current_node.children[-1].feasible = False
                    continue
                print("Feasible action")
                
                # If the action is feasible, add it to the queue
                queue.append(current_node.children[-1])
                
        # If the loop terminates without finding a solution
        print("No solution found")
        return None