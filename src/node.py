import networkx as nx
import matplotlib.pyplot as plt
from networkx.drawing.nx_pydot import graphviz_layout

class Node:
    
    def __init__(self, choice_of_number, row, col, parent, node_id = 0, depth = 0):
        # Node identity related properties
        self.node_id = node_id
        self.depth = depth
        self.feasible = True
        
        # Tree structure related
        self.parent = parent
        self.children = []
        
        # Action/State related variables
        self.action = choice_of_number
        self.row = row
        self.col = col
    
    def add_child(self, choice_of_number, row, col, total_number_of_choices):
        node_id = self.node_id * total_number_of_choices + len(self.children) + 1
        #print(f"Current node ID: {self.node_id:3d}, Number of Children: {choice_of_number:3d},Node ID: {node_id:3d}")
        
        child_node = Node(choice_of_number, row, col, self, node_id, self.depth + 1)
        self.children.append(child_node)

        return child_node
        
    def print(self):
        # Print command which treats the current node as the root node

        def print_recursive(node):
            print(f"Node ID: {node.node_id}")
            print(f"\tDepth: {node.depth}, Action: {node.action}, Row: {node.row}, Col: {node.col}")
            print(f"\tChildren: {[child.node_id for child in node.children]}\n")
            
            for child in node.children:
                print_recursive(child)
        
        print_recursive(self)
        
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
    
    