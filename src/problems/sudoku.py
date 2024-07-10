from src.primitives.problem import Problem

class Sudoku(Problem):
    
    def __init__(self, board, goal_state):
        self.board = board
        self.goal_state = goal_state
        
        
    def apply_action(self, action):
        i, j, value = action
        self.board[i][j] = value
    
    def is_valid_move(self, i, j, value):
        # Check if the value is not in the row
        for column in range(9):
            if self.board[i][column] == value:
                return False

        # Check if the value is not in the column
        for row in range(9):
            if self.board[row][j] == value:
                return False

        # Check if the value is not in the 3x3 grid
        startRow, startCol = 3 * (i // 3), 3 * (j // 3)
        for row in range(startRow, startRow + 3):
            for col in range(startCol, startCol + 3):
                if self.board[row][col] == value:
                    return False

        return True
        
    
    def get_valid_moves(self):
        valid_actions = []
        check: bool = False
        for i in range(9):
            for j in range(9):
                if self.board[i][j] == 0:
                    for value in range(1, 10):
                        if self.is_valid_move(i, j, value):
                            valid_actions.append((i, j, value))
                            check = True
                        
                        if check:
                            break   
                if check:
                    break
            if check:
                break
        
        # print(len(valid_actions))
        # print("Valid actions: ", valid_actions)
        return valid_actions
    
    def get_score(self):
        score = 0
        
        # Check rows
        for i in range(9):
            row = set(self.board[i])
            if len(row) == 9:
                score += 9
        
        # Check columns
        for j in range(9):
            column = set(self.board[i][j] for i in range(9))
            if len(column) == 9:
                score += 9
        
        # Check 3x3 grids
        for i in range(0, 9, 3):
            for j in range(0, 9, 3):
                grid = set(self.board[x][y] for x in range(i, i+3) for y in range(j, j+3))
                if len(grid) == 9:
                    score += 9
        
        return score
    
    def is_game_finished(self):
        return self.board == self.goal_state
    
    def get_game_state(self):
        return self.board
    
    def load_game_from_state(self, state):
        self.board = state
        
    def pprint(self):
        for row in self.board:
            print(row)
        print("\n")
        