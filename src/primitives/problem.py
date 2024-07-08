"""

This is a problem.py class with explicit handling for
Constraint Satisfaction Problems.

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
    # These will be used by the solvers

    # play action functions

    def play_action(self, action_id):
        action = self.action_id_to_action(action_id)
        self.apply_action(action)

    def play_actions(self, action_ids):
        for action_id in action_ids:
            self.play_action(action_id)

    # This function doesn't make a lot of sense
    # as how can he solver know what actions it
    # can possibly make without consulting the actual
    # problem class.
    # Not Recommended using this.
    def play_actions_from_scratch(self, action_ids):
        self.clear()
        self.play_actions(action_ids)
        
    # info functions
    def get_number_of_actions(self):
        pass

    # USER - IMPLEMENTABLE Functions

    def clear(self):
        pass

    def apply_action(self):
        pass

    # converter functions
    def action_id_to_action(self, action_id):
        pass

    def action_to_action_id(self, action):
        pass
