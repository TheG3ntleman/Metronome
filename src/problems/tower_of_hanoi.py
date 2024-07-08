from src.primitives.problem import Problem

class TowerOfHanoi(Problem):
    
    def __init__(self, number_of_disks = 3, number_of_pegs = 3):
        self.number_of_disks = number_of_disks
        self.number_of_pegs = number_of_pegs
        
        self.pegs = [[i for i in range(number_of_disks, 0, -1)]] + [[] for _ in range(number_of_pegs - 1)]

        self.goal_state = [[] for _ in range(number_of_pegs - 1)] + [[i for i in range(number_of_disks, 0, -1)]]
    
    def apply_action(self, action):
        from_peg, to_peg = action
        disk = self.pegs[from_peg].pop()
        self.pegs[to_peg].append(disk)

    def get_valid_moves(self):
        valid_actions = []
        
        for from_peg in range(self.number_of_pegs):
            for to_peg in range(self.number_of_pegs):
                if len(self.pegs[from_peg]) > 0 and (len(self.pegs[to_peg]) == 0 or self.pegs[to_peg][-1] > self.pegs[from_peg][-1]):
                    valid_actions.append((from_peg, to_peg))
        
        return valid_actions

    def get_score(self):
        penalty = 0
        for i in range(self.number_of_pegs - 1):
            penalty += len(self.pegs[i]) ** 2
        return len(self.pegs[-1]) - penalty
    
    def is_game_finished(self):
        return self.pegs == self.goal_state

    def get_game_state(self):
        return self.pegs

    def load_game_from_state(self, state):
        self.pegs = state

    def pprint(self):
        for peg in self.pegs:
            print(peg)
        print("\n")
