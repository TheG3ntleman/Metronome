from src.primitives.problem import Problem

class Sudoku(Problem):
    
    def __init__(self, board):
        self.board = board
        
        
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
                    check = True
                    for value in range(1, 10):
                        if self.is_valid_move(i, j, value):
                            valid_actions.append((i, j, value))
                            
                if check:
                    break
            if check:
                break
            
        return valid_actions
    
    def get_score(self):
        score = 0
        # Check rows and columns
        for i in range(9):
            row_set = set()
            col_set = set()
            for j in range(9):
                if self.board[i][j] != 0:
                    row_set.add(self.board[i][j])
                if self.board[j][i] != 0:
                    col_set.add(self.board[j][i])
            score += len(row_set) + len(col_set)

        # Check 3x3 subgrids
        for x in range(0, 9, 3):
            for y in range(0, 9, 3):
                subgrid_set = set()
                for i in range(3):
                    for j in range(3):
                        if self.board[x+i][y+j] != 0:
                            subgrid_set.add(self.board[x+i][y+j])
                score += len(subgrid_set)

        return score
        
    
    def is_game_finished(self):
        # Check if the board is filled
        for i in range(9):
            for j in range(9):
                if self.board[i][j] == 0:
                    return False
        
        # Check if the board is valid
        for i in range(9):
            for j in range(9):
                value = self.board[i][j]
                if not self.is_valid_move(i, j, value):
                    return False
        
        return True
    
    def get_game_state(self):
        return self.board
    
    def load_game_from_state(self, state):
        self.board = state
        
    def pprint(self):
        for row in self.board:
            print(row)
        print("\n")
        