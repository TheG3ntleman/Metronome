from math import sqrt, log
from copy import deepcopy
from random import randint
from src.sudoku import Sudoku, SudokuRules
from tqdm import tqdm

class MultiArmBanditSolver:
    
    def __init__(self, puzzle):
        self.puzzle = puzzle
        self.number_of_possible_actions = self.puzzle.puzzle_size

    
    def run(self, simulation_per_step = 100, exploration_parameter = 2 ** 0.5):
        
        # This 'run' implementation has no 'jump back'
        
        # First listing all the grid points at which decisions
        # must be made
        points_of_actions = []
        for i in range(self.puzzle.puzzle_size):
            for j in range(self.puzzle.puzzle_size):
                if self.puzzle.grid[i][j] == 0:
                    points_of_actions.append((i, j))
                    
        
        # Iterating through the decision points
        
        number_of_actions_taken = 0
        
        for point_i, point_of_action in enumerate(points_of_actions):
            print(f"\nCurrent point of action : {point_i} / {len(points_of_actions)}")
            # STEP - 0: Checking feasible actions at this point
            feasible_actions = []
            for option in range(1, self.number_of_possible_actions + 1):
                #if self.puzzle.is_feasible_action(point_of_action[0], point_of_action[1], option):
                #    feasible_actions.append(option)
                
                if SudokuRules.is_valid(self.puzzle.grid, self.puzzle.size, point_of_action[0], point_of_action[1], option):
                    feasible_actions.append(option)
                    
            if len(feasible_actions) == 0:
                print("Failed to find a solution")
                return False, number_of_actions_taken # Failed to find a solution
            
            print("Feasible actions: ", feasible_actions)
            
            if len(feasible_actions) == 1:
                self.puzzle.grid[point_of_action[0]][point_of_action[1]] = feasible_actions[0]
                number_of_actions_taken += 1
                continue
            
            
            # STEP - 1: Running multiple simulations to sample distribution
            # of rewards for each action
            rewards = [0] * len(feasible_actions)
            max_rewards = [0] * len(feasible_actions)
            visits = [0] * len(feasible_actions)
            
            for i_simulation in tqdm(range(simulation_per_step)):
                # Calculating UCB value for each action
                current_option = 0
                max_ucb_value = -1
                for i_option in range(len(feasible_actions)):
                    if (visits[i_option] == 0):
                        current_option = i_option
                        break
                    
                    ucb_value = (rewards[i_option] / visits[i_option]) + exploration_parameter * sqrt(log(i_simulation) / visits[i_option])
                    
                    if ucb_value > max_ucb_value:
                        max_ucb_value = ucb_value
                        current_option = i_option

                # We now sample the reward for the current option
                reward = self.sample_reward(point_of_action, points_of_actions[point_i + 1:], feasible_actions[current_option])
                
                # Updating the reward and visits
                rewards[current_option] += reward
                max_rewards[current_option] = max(reward, max_rewards[current_option])
                visits[current_option] += 1
                
            # Selecting the best action
            best_option = 0
            max_avg_reward = -1
            
            for i_option in range(len(feasible_actions)):
                if rewards[i_option] / visits[i_option] > max_avg_reward:
                    max_avg_reward = rewards[i_option] / visits[i_option] # max_rewards[i_option] / rewards[i_option] / visits[i_option]
                    best_option = i_option
            
            # Applying the best action
            print(f"Applying action: {feasible_actions[best_option]}")
            print("Rewards: ", rewards)
            print("Visits: ", visits)
            print("Average rewards: ", [rewards[i] / visits[i] for i in range(len(feasible_actions))])
            print("Max rewards: ", max_rewards)
            
            self.puzzle.grid[point_of_action[0]][point_of_action[1]] = feasible_actions[best_option]
            number_of_actions_taken += 1
            
        return True, number_of_actions_taken
    
    def sample_reward(self, point_of_action, further_points_of_action, option):
        # There are many approaches to sample the reward
        # here we will just do a flat sample
        
        # Make a copy of the puzzle grid
        grid = deepcopy(self.puzzle.grid)
        grid[point_of_action[0]][point_of_action[1]] = option
        
        # We now iterate through the further points of action
        achieved_depth = 0
        
        for depth, point_of_action in enumerate(further_points_of_action):
            # STEP - 0: Checking feasible actions at this point
            feasible_actions = []
            for option in range(1, self.number_of_possible_actions + 1):
                #if self.puzzle.is_feasible_action(point_of_action[0], point_of_action[1], option):
                #    feasible_actions.append(option)
                
                if SudokuRules.is_valid(grid, self.puzzle.size, point_of_action[0], point_of_action[1], option):
                    feasible_actions.append(option)
                    
            if len(feasible_actions) == 0:
                achieved_depth = depth
                break
            
            # Randomly selecting an action
            if len(feasible_actions) == 1:
                action = feasible_actions[0]
            else:
                action = feasible_actions[randint(0, len(feasible_actions) - 1)]
            
            # Applying the action
            grid[point_of_action[0]][point_of_action[1]] = action
            
            # Checking if this is the last iteration
            if (depth == len(further_points_of_action) - 1):
                achieved_depth = depth
        
        # Calculating reward from depth, in this case to 
        # keep things simple the depth itself will be viewed as
        # the reward
        
        return achieved_depth
        