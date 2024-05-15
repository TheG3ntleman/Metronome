import networkx as nx
import matplotlib.pyplot as plt
from networkx.drawing.nx_pydot import graphviz_layout

from tqdm import tqdm_gui, tqdm
from collections import deque
from copy import deepcopy
import random
count = 1

class TreeSearch:
    
    """class Node:
    
        def __init__(self, action, parent, node_id, row, col):
            
            self.node_id = node_id
            self.action = action
            self.feasible = True

            self.parent = parent           
            self.children = []
            
            self.row = row
            self.col = col
            
            self.count = 0 

        def add_child(self, action, row, col):
            child = TreeSearch.Node(action, self, len(self.children) + self.node_id + 1, row, col)
            print("ID of child:", child.node_id)
            self.children.append(child)
        
        def to_networkx(self):
            # Function to treat the current node as a root node
            # and construct a tree as a networkx directed graph from it.
            
            G = nx.DiGraph()
            node_colors = []
            
            self.to_networkx_add_node(self, G, node_colors)
            self.to_networkx_add_children_recursively(G, node_colors)
            
            return G, node_colors
            
        def to_networkx_add_children_recursively(self, G, node_colors):
            for child in self.children:
                # Adding node to the graph
                self.to_networkx_add_node(child, G, node_colors)
                # Adding edge from parent to child
                G.add_edge(self.node_id, child.node_id)
                child.to_networkx_add_children_recursively(G, node_colors)
        
        def to_networkx_add_node(self, node, G, node_colors):
            self.count += 1
            G.add_node(node.node_id)
            print(f"node colors: {node_colors}, count: {self.count}")
            node_colors.append('blue' if node.feasible else 'black')
            print(f"node colors: {node_colors}")

        
        def plot(self):
            # Retrieving the corresponding networkx graph
            G, node_colors = self.to_networkx()
            
            print(f"NODES AND COLORS: {G.number_of_nodes()}/{len(node_colors)}")
            
            # Plot the graph using graphviz layout
            pos = graphviz_layout(G, prog='dot')
            nx.draw(G, pos, with_labels=True, arrows=True)#, node_color=node_colors, font_color='red')
            plt.show()"""


    class Node:
        
        def __init__(self, action, parent, node_id, row, col):
            self.node_id = node_id
            self.action = action
            self.feasible = True
            
            self.parent = parent           
            self.children = []
            
            self.row = row
            self.col = col

        def add_child(self, action, row, col):
            global count
            child = TreeSearch.Node(action, self, count, row, col)#len(self.children) + self.node_id + 1, row, col)
            count += 1
            self.children.append(child)


        def plot(self):
            pass
        
        def print(self):
            # Printing via DFS 
            self.print_children_recursive(self)
        
        def print_children_recursive(self, node):
            print(f"Node Id: {node.node_id:3d}, row: {node.row}, column: {node.col}, Action: {node.action:3d}, Ids of child nodes: {[child.node_id for child in node.children]}")
            for child in node.children:
                self.print_children_recursive(child)
                
        def to_networkx(self):
            # Function to treat the current node as a root node
            # and construct a tree as a networkx directed graph from it.
            
            G = nx.DiGraph()
            node_colors = []
            
            self.to_networkx_add_node(self, G, node_colors)
            self.to_networkx_add_children_recursively(G, node_colors)
            
            return G, node_colors
            
        def to_networkx_add_children_recursively(self, G, node_colors):
            for child in self.children:
                # Adding node to the graph
                self.to_networkx_add_node(child, G, node_colors)
                # Adding edge from parent to child
                G.add_edge(self.node_id, child.node_id)
                child.to_networkx_add_children_recursively(G, node_colors)
        
        def to_networkx_add_node(self, node, G, node_colors):
            G.add_node(node.node_id)
            node_colors.append('blue' if node.feasible else 'black')
            

        def plot(self):
            
            # Retrieving the corresponding networkx graph
            G, node_colors = self.to_networkx()
            
            # Plot the graph using graphviz layout
            pos = graphviz_layout(G, prog='dot')#hierarchy_pos(G,0) 
            nx.draw(G, pos, with_labels=True, arrows=True, node_color=node_colors, font_color='red')
            plt.show()

        
        
    
    def __init__(self, puzzle):
        self.root_node = TreeSearch.Node(0, None, 0, 0, 0)
        self.puzzle = puzzle
    
    def get_to_next_empty_cell(self, curr_row, curr_col):
        # Getting to the next empty cell
        row, col = curr_row, curr_col
        while row < self.puzzle.puzzle_size and self.puzzle.grid[row][col] != 0:
            col += 1
            if col == self.puzzle.puzzle_size:
                col = 0
                row += 1
            if row == self.puzzle.puzzle_size:
                break
        print(f"Going to next empty cell: {row}, {col}")
        return row, col
                
    """def bfs(self, number_of_nodes_explored = 100):
        queue = deque()
        
        # Initializing the row and column
        row, col = 0, 0
        
        if self.puzzle.grid[row][col] != 0:
            row, col = self.get_to_next_empty_cell(row, col)
                
        # Adding the root node
        queue.append(self.root_node)
        
        # Running the main loop for BFS
        for _ in tqdm(range(number_of_nodes_explored), total = number_of_nodes_explored, desc = "Number of Nodes currently explored"):
            
            # Breaking algorithm if the queue is empty
            if len(queue) == 0:
                print("Explored full search space.")
                break
            
            # Pop the first element from the queue
            for node in queue:
                print(node.action, end = " ")
            node = queue.popleft()
            
            # Adding children to the node
            for action in range(1, self.puzzle.puzzle_size + 1):
                node.add_child(action, row, col)
                
                current_node = node.children[-1]
                
                # Checking for feasibility
                # Reconstructing the puzzle
                puzzle = deepcopy(self.puzzle)
                
                # Iterating up the branch to the root node
                while current_node.parent is not None:
                    puzzle.grid[current_node.row][current_node.col] = current_node.action
                    current_node = current_node.parent
                
                puzzle.print_puzzle()
                
                # Checking if the puzzle is feasible
                if not self.puzzle.rules.is_valid(row, col, action):
                    print(f"row: {row}, col: {col}, action: {action} is infeasible.")
                    node.children[-1].feasible = False
                    continue
                
                # Adding the child to the queue
                queue.append(node.children[-1])        
                
                node.children[-1].feasible = False
            
            # Jumping to next position for action
            row, col = self.get_to_next_empty_cell(row, col)"""
    
    def bfs(self, number_of_nodes_explored=100):
        queue = deque()
        
        # Initializing the row and column
        row, col = 0, 0
        
        if self.puzzle.grid[row][col] != 0:
            row, col = self.get_to_next_empty_cell(row, col)
            
        # Adding the root node
        queue.append(self.root_node)
        
        # Running the main loop for BFS
        for _ in tqdm(range(number_of_nodes_explored), total=number_of_nodes_explored, desc="Number of Nodes currently explored"):
            
            # Breaking algorithm if the queue is empty
            if len(queue) == 0:
                print("Explored full search space.")
                break
            
            # Pop the first element from the queue
            node = queue.popleft()
            row, col = node.row, node.col
            
            # Adding children to the node
            for action in range(1, self.puzzle.puzzle_size + 1):
                node.add_child(action, row, col)
                
                current_node = node.children[-1]
                
                # Checking for feasibility
                # Reconstructing the puzzle
                puzzle = deepcopy(self.puzzle)
                
                # Iterating up the branch to the root node
                while current_node.parent is not None:
                    puzzle.grid[current_node.row][current_node.col] = current_node.action
                    current_node = current_node.parent
                
                puzzle.print_puzzle()
                
                # Checking if the puzzle is feasible
                if not self.puzzle.rules.is_valid(row, col, action):
                    print(f"row: {row}, col: {col}, action: {action} is infeasible.")
                    node.children[-1].feasible = False
                    continue
                
                # Adding the child to the queue
                queue.append(node.children[-1])        
                
            
            # Jumping to next position for action
            col += 1
            if col == self.puzzle.puzzle_size:
                col = 0
                row += 1
            if row == self.puzzle.puzzle_size:
                print("Found a solution.")
                break
            while row < self.puzzle.puzzle_size and self.puzzle.grid[row][col] != 0:
                col += 1
                if col == self.puzzle.puzzle_size:
                    col = 0
                    row += 1
                if row == self.puzzle.puzzle_size:
                    print("Found a solution.")
                    break
            print(f"Going to next empty cell: {row}, {col}")


