from metronome.common.reduced_problem import Problem
from metronome.common.timetable import TimeTable	
from metronome.common.constraint import Constraints

# defining hyperparameters here
max_iterations = 500


# node class acts as a node for a tree and in a way keeps track of the tree itself
class Node:
    def __init__(self,timeslot_venue_tuple, depth, parent=None):
        self.feasible = True
        self.timeslot_venue_tuple = timeslot_venue_tuple
        self.depth = depth
        self.parent = parent
        self.children = []
        self.visit_count = 0
        self.average_violations = 0

    def selection_policy(self, node):
        
        return node
    
    def back_propogation(self, violations, node):
        
        pass
    
    def add_child(self, child):
        self.children.append(child)


class MCTS:
    def __init__(self, time_table_specifications, reduced_problem, K):
        self.k = K
        self.time_table_specifications = time_table_specifications
        self.reduced_problem = reduced_problem
        self.partial_timetable = TimeTable(self.time_table_specifications.sessions_table['size'])
        self.constraints = Constraints(time_table_specifications= self.time_table_specifications)

    def execute_MCTS(self):
        # declaring the root of the tree
        root_node = Node(None, -1, None)
        current_node = root_node

        # run the Mcts till current node reaches the end of the time table
        while current_node.depth < self.time_table_specifications.sessions_table['size'] - 1:
            
            # adding children to the current node
            for i in range(self.k):
              child_node = Node(self.reduced_problem[current_node.depth + 1][i], current_node.depth + 1, current_node)
              current_node.add_child(child_node)

            # now one epoch of Mcts is executed
            iterations = 0
            while iterations < max_iterations:
                # applying selection policy to get the node in the tree to expand from
                current_node.selection_policy(current_node)

                # checking if the current_node results has any feasible children 
                feasible_children = []
                for child in current_node.children:
                    # updating the partial timeatable to pass it to is_feasible function
                    self.partial_timetable.schedule_session(child.depth, child.timeslot_venue_tuple[0], child.timeslot_venue_tuple[1])
                    # checking if adding the child makes the partial time table fessible or not
                    if self.constraints.is_feasible(self.partial_timetable, child.depth) and child.feasible == True:
                        feasible_children.append(child)
                
                if not feasible_children:
                    current_node.feasible = False
                    current_node.children = []
                    # as the current node is infeasible we go to parent and check feasibility
                    while current_node.parent != None:
                        current_node = current_node.parent
                        for child in current_node.children:
                          # updating the partial timeatable to pass it to is_feasible function
                          self.partial_timetable.schedule_session(child.depth, child.timeslot_venue_tuple[0], child.timeslot_venue_tuple[1])
                          # checking if adding the child makes the partial time table fessible or not
                          if self.constraints.is_feasible(self.partial_timetable, child.depth) and child.feasible == True:
                              feasible_children.append(child)
                        # if current node is not feasible we set feasible field as false
                        if not feasible_children:
                            current_node.feasible = False
                            current_node.children = []
                        # else we break the while loop
                        else:
                            break 
                    # as the selcted node was infeasible we dont execute roll outs and back_propogation  
                    continue
                
                # now we execute roll outs
                     
                        



    
