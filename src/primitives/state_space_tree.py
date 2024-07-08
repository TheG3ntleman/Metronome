"""

This file contains a class for a state space
tree node with explicit support for constrain
satisfaction problems (CSPs).

"""

class StateSpaceTreeNode:

    def __init__(self, action, parent = None):
        self.action = action
        self.parent = parent
        self.children = []
        self.properties = {}

        if parent is not None:
            self.depth = parent.depth + 1
        else:
            self.depth = 0
    
    def add_child(self, child):
        self.children.append(child)

    def number_of_children(self):
        return len(self.children)
    
    def get_actions(self):
        # Will be stored in order of oldest action to most
        # recent.
        actions = []
        
        def get_action_recursively(node):
            # Root node's action never gets recorded
            # as it by convention corresponds to an
            # empty sequence of actions.
            if node.parent is not None:
                get_action_recursively(node.parent)
                actions.append(node.action)
        
        get_action_recursively(self)

        return actions