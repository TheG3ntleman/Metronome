from metronome.common import Constraints
from metronome.timetable import TimeTable
import math
import random
import copy

# declaring hyperparameters and variables
simulation_hyper_parameter = 3
no_feasible_child_penalty = 200
max_iterations = 1000

# node class acts as a node for a tree
class Node:
    def __init__(self,timeslot_venue_tuple, depth, parent=None):
        self.feasible = True
        self.timeslot_venue_tuple = timeslot_venue_tuple
        self.depth = depth
        self.parent = parent
        self.children = []
        self.visit_count = 0
        self.cummulative_violations = 0

    def add_child(self, child):
        self.children.append(child)

class MCTS_solver:

    def __init__(self, k, time_table_specifications,mcts_tuple_frequency_problem):
        self.k = k
        self.root = Node(None, -1, None)
        self.time_table_specifications = time_table_specifications
        self.constraints = Constraints(time_table_specifications= self.time_table_specifications)
        self.mcts_tuple_frequency_problem = mcts_tuple_frequency_problem


    def initialize_tree(self):
        # creating a partial timetable to keep track of things
        self.partial_timetable = TimeTable(self.time_table_specifications.number_of_sessions)
        # initializing the roots children
        for i in range(self.k):
            child_node = Node(self.mcts_tuple_frequency_problem[0][i], 0, self.root)
            self.root.add_child(child_node)


    def selection_policy(self, node):
        # itrating over all nodes in tree too find the best one to expand
        while node.depth < self.time_table_specifications.number_of_sessions - 1:

            # seeing if a nodes children have not been visited
            for i in range(self.k):
                # if nodes children have not been visited then using them to expand
                if node.children[i].visit_count == 0:
                    # updating the partial timeatable to pass it to the is_feasible function
                    self.partial_timetable.set_session_info(node.children[i].depth, node.children[i].timeslot_venue_tuple[0], node.children[i].timeslot_venue_tuple[1])
                    # checking if the children not visited gives rise to a feasible partial time table
                    if self.constraints.is_feasible(self.partial_timetable, node.children[i].depth):
                        #print("the node selected is: ", node.children[i].timeslot_venue_tuple)
                        #node.children[i].visit_count += 1
                        return node.children[i]
            
            # if all children have been visited then using UCB formula to find the best one to select
            feasible_children = []
            for child in node.children:
                # updating the partial timeatable to pass it to is_feasible function
                self.partial_timetable.set_session_info(child.depth, child.timeslot_venue_tuple[0], child.timeslot_venue_tuple[1])
                # checking if adding the child makes the partial time table fessible or not
                if self.constraints.is_feasible(self.partial_timetable, child.depth) and child.feasible == True:
                    feasible_children.append(child)
            
            # if no feasible children then returning the node we are at so violations can be backpropogated
            if not feasible_children:
                # removing current node children as in simulation we have to add them
                print("this node is not feasible: ", node.timeslot_venue_tuple)
                node.feasible = False
                node.children = []
                return node
            
            # using UCB formula to choose best child
            best_ucb = float('inf')
            best_child = None
            for child in feasible_children:
                ucb = self.calculate_ucb(child)
                # selcting the lowest as its a minimization problem
                if ucb < best_ucb:
                    best_ucb = ucb
                    best_child = child
            
            # going down the tree to find the best node to expand
            node = best_child
            # updating partial timetable
            self.partial_timetable.set_session_info(node.depth, node.timeslot_venue_tuple[0], node.timeslot_venue_tuple[1])
        
        # if while loop ends without return i.e. we have reached a feasible & optimmal solution
        return node
                

    def calculate_ucb(self, node):
        # handling exceptions
        if node.visit_count == 0:
            return float('inf')
        # calculating exploitation and exploration
        exploitation = node.cummulative_violations / node.visit_count
        exploration = math.sqrt(4 * math.log(node.parent.visit_count) / node.visit_count)
        return exploitation + exploration
                
    
    def simulation_policy(self, node):
        
        # finding the best route from current node to reach maximum depth and calculating the reward
        while node.depth < self.time_table_specifications.number_of_sessions - 1:
            # generating the children for the node
            for i in range(self.k):
                child_node = Node(self.mcts_tuple_frequency_problem[node.depth + 1][i], node.depth + 1, node)
                node.add_child(child_node)
            
            # finding the feasible nodes to choose out of all the children
            feasible_children = []
            for child in node.children:
                # updating the partial timeatable to pass it to is_feasible function
                self.partial_timetable.set_session_info(child.depth, child.timeslot_venue_tuple[0], child.timeslot_venue_tuple[1])
                # checking if adding the child makes the partial time table fessible or not
                if self.constraints.is_feasible(self.partial_timetable, child.depth) and child.feasible == True:
                    feasible_children.append(child)

            # if no feasible children then returning the node we are at so violations can be backpropogated
            if not feasible_children:
                node.children = []
                return node
            
            # adding randomness and exploitation of soft constraint optimization to simulation
            p = random.randint(0,9)

            if p >= simulation_hyper_parameter:
                # iterating over the feasible children to find the most optimal one
                minimum = float('inf')
                best_child = None
                for child in feasible_children:
                    # updating partial timetable to see if which child is optimal
                    self.partial_timetable.set_session_info(child.depth, child.timeslot_venue_tuple[0], child.timeslot_venue_tuple[1])
                    # checking the current child is optimal, note: it is a minimzation problem as we are looking at violations
                    if minimum > self.constraints.evaluate_soft_constraints(self.partial_timetable, child.depth):
                        minimum = self.constraints.evaluate_soft_constraints(self.partial_timetable, child.depth)
                        best_child = child
            else:
                best_child = feasible_children[random.randint(0,len(feasible_children)-1)]

            # updating the node to move to next child in tree
            node = best_child
            # updating partial timetable
            self.partial_timetable.set_session_info(node.depth, node.timeslot_venue_tuple[0], node.timeslot_venue_tuple[1])
        
        # if we are out of the while loop that means the simulation has been done and we ahve reached the last session
        return node
    
    '''
    def back_propogate(self, violations, added_node, node):
        f = copy.deepcopy(node.depth)
        # Backpropagate violations up the tree
        # handling two different cases
        # 1) when simulation reaches the maximum depth then we just back propogate the violations
        if node.depth == self.time_table_specifications.number_of_sessions - 1:
            # back propogating the violations
            added_node.children = []
            # adding children
            for i in range(self.k):
                child_node = Node(self.mcts_tuple_frequency_problem[added_node.depth + 1][i], added_node.depth + 1, added_node)
                added_node.add_child(child_node)
            while added_node is not None:
                added_node.visit_count += 1
                added_node.cummulative_violations += violations
                # if the current node is at the node that was added by the selection policy then we remove it,
                # as they are part of simulation and add new children 
                added_node = added_node.parent
        elif added_node.depth == node.depth:
            added_node.feasible = False
            added_node.children = []
            # adding children
            for i in range(self.k):
                child_node = Node(self.mcts_tuple_frequency_problem[added_node.depth + 1][i], added_node.depth + 1, added_node)
                added_node.add_child(child_node)
            while added_node is not None:
                added_node.visit_count += 1
                added_node.cummulative_violations += 500*violations*(self.time_table_specifications.number_of_sessions - added_node.depth - 1)
                
                added_node = added_node.parent
        # 2) when simulation does not reach the maximum depth we multiply violations with a number proportional
        #    to the depth of the last node simulation reached(THE LEGENDS FORMULA)
        else:
            added_node.children = []
            # adding children
            for i in range(self.k):
                child_node = Node(self.mcts_tuple_frequency_problem[added_node.depth + 1][i], added_node.depth + 1, added_node)
                added_node.add_child(child_node)
            while added_node is not None:
                added_node.visit_count += 1
                added_node.cummulative_violations += violations*(self.time_table_specifications.number_of_sessions - added_node.depth - 1)#/(f - added_node.depth + 2)
                # if the current node is at the node that was added by the selection policy then we remove it,
                # as they are part of simulation and add new children 
                added_node = added_node.parent
    '''
    def back_propogate(self, violations, added_node, node):

        # Backpropagate violations up the tree
        # handling two different cases
        
        # 1) when simulation reaches the maximum depth then we just back propogate the violations
        if node.depth == self.time_table_specifications.number_of_sessions - 1:
            # back propogating the violations
            while node is not None:
                node.visit_count += 1
                node.cummulative_violations += violations
                # if the current node is at the node that was added by the selection policy then we remove it,
                # as they are part of simulation and add new children 
                if added_node.depth == node.depth:
                    node.children = []
                    # adding children
                    for i in range(self.k):
                        child_node = Node(self.mcts_tuple_frequency_problem[added_node.depth + 1][i], added_node.depth + 1, added_node)
                        node.add_child(child_node)
                node = node.parent
        
        # 2) when simulation does not reach the maximum depth we multiply violations with a number proportional
        #    to the depth of the last node simulation reached and also a penalty(as the node is pretty bad)
        elif added_node.depth == node.depth:
            while node is not None:
                node.visit_count += 1
                node.cummulative_violations += no_feasible_child_penalty*violations*(self.time_table_specifications.number_of_sessions - added_node.depth - 1)
                # if the current node is at the node that was added by the selection policy then we remove it,
                # as they are part of simulation and add new children 
                if added_node.depth == node.depth:
                    node.children = []
                    # adding children
                    for i in range(self.k):
                        child_node = Node(self.mcts_tuple_frequency_problem[added_node.depth + 1][i], added_node.depth + 1, added_node)
                        node.add_child(child_node)
                node = node.parent
        
        # 3) when simulation does not reach the maximum depth we multiply violations with a number proportional
        #    to the depth of the last node simulation reached(THE LEGENDS FORMULA)
        else:
            while node is not None:
                node.visit_count += 1
                node.cummulative_violations += violations*(self.time_table_specifications.number_of_sessions - added_node.depth - 1)#/(f - added_node.depth + 2)
                # if the current node is at the node that was added by the selection policy then we remove it,
                # as they are part of simulation and add new children 
                if added_node.depth == node.depth:
                    node.children = [] 
                    # adding the children
                    for i in range(self.k):
                        child_node = Node(self.mcts_tuple_frequency_problem[added_node.depth + 1][i], added_node.depth + 1, added_node)
                        node.add_child(child_node)
                node = node.parent    
                   
    def run_MCTS(self):

        # declaring iteration counter
        iterations = 0
        # initializint the tree and current node
        self.initialize_tree()
        current_node = self.root
        n_solutions = 0

        while n_solutions < 1:
            iterations += 1
            # selection policy
            current_node = self.selection_policy(current_node)

            # adding the termination condition
            if current_node.depth == self.time_table_specifications.number_of_sessions - 1:
                n_solutions += 1
                print("the SOLUTION is: \n")
                self.partial_timetable.print()
                print("number of iterations to run mcts:",iterations)
                continue

            # remembring the new added child in added node
            added_node = copy.deepcopy(current_node)
            # printing the newest selected child from the simulation policy
            print("the newest node that is added to the tree is at depth: ",added_node.depth)
            print("and it is: ",added_node.timeslot_venue_tuple)
            print("feasibility of current node: ", current_node.feasible)

            # simulation policy
            if current_node.feasible == True:
                print("the current_node is feasible")
                current_node = self.simulation_policy(current_node)

            # calculation soft constraint violations using the above current node
            violations = self.constraints.evaluate_soft_constraints(self.partial_timetable, current_node.depth)

            # printing the last node reached by the simulation
            print("the last node reached from simulation is: ", current_node.timeslot_venue_tuple, violations)
            
            # back propogation
            self.back_propogate(violations, added_node, current_node)
            
            # re initializing the current node and partial time table for the next iteration to run
            current_node = self.root
            for i in range(self.time_table_specifications.number_of_sessions):
                self.partial_timetable.set_session_info(i, 0, 0)

            # if iterations greater than max iteration then restart the whole algorithm
            if iterations > max_iterations:
                self.root = Node(None, -1, None)
                self.initialize_tree()
                current_node = self.root
                iterations = 0