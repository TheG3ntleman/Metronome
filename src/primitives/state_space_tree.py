"""

This file contains a class for a state space
tree node with explicit support for constrain
satisfaction problems (CSPs).

"""

import matplotlib.pyplot as plt
from networkx.drawing.nx_pydot import graphviz_layout
import networkx as nx

number_of_nodes = 0

class StateSpaceTreeNode:

    def __init__(self, action, parent = None):
        global number_of_nodes
        self.action = action
        self.parent = parent
        self.children = []
        self.properties = {}

        if parent is not None:
            self.depth = parent.depth + 1
        else:
            self.depth = 0

        self.node_id = number_of_nodes
        self.feasible = True
        number_of_nodes += 1
    
    def add_child(self, child):
        self.children.append(child)

    def get_number_of_children(self):
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
        

    def is_leaf(self):
        return len(self.children) == 0
    
    # Everything below is for plotting
    
    def to_networkx(self):
        # Generate this tree as a networkx digraph with this as the root node
        tree = nx.DiGraph()
        node_colors = []
        
        def add_node(node):
            tree.add_node(node.node_id)
            node_colors.append('blue' if node.feasible else 'black')
        
        def add_children_recursively(node):
            for child in node.children:
                add_node(child)
                tree.add_edge(node.node_id, child.node_id)
                add_children_recursively(child)
        
        add_node(self)
        add_children_recursively(self)

        return tree, node_colors
        
    def plot(self):
        # Generate the plot taking this node as root.
        
        tree, node_colors = self.to_networkx()
        pos = graphviz_layout(tree, prog='dot')
        nx.draw(tree, pos, with_labels=False, arrows=True, node_color=node_colors, font_color='red')
        plt.show()