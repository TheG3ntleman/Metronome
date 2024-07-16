"""

This is a problem.py class with explicit handling for
Constraint Satisfaction Problems.

"""

"""
getActionSize
getValidMoves
getGameEnded
%getGameStringRepresentation
getScore % Probably only works on terminal states.
getPenalty % When the game is not over, but the player can't take
            % any more moves.

Some Problems to resolve:
    - Must decide on whether we are using the term 'action' or 'move'.
    - Must return state of the game in functions, i.e the internal state of a game should not be maintained in the class.
"""

class Problem:
    # The functions in this class can be put into
    # two categories: (a) functions that are common
    # to all problems (which are fully implemented)
    # functions specific to problems which are not
    # implemented but are heavily used by the written
    # functions.

    def __init__(self):
        pass

    # COMMON - IMPLEMENTED Functions (operate on action_id even though specified otherwise)

    # play action functions

    def play_action(self, action):
        self.apply_action(action)

    def play_actions(self, actions):
        for action in actions:
            self.apply_action(action)

    # USER - IMPLEMENTABLE Functions

    def clear(self):
        pass

    def apply_action(self):
        pass

    def get_valid_moves(self):
        pass

    def get_score(self):
        pass

    def get_penalty(self):
        return 0

    def is_game_finished(self):
        pass

    def save_state(self):
        pass # Must return some "state"

    def load_state(self, state):
        pass

    def pprint(self):
        pass

