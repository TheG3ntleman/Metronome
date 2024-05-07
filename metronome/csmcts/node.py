from metronome.common.timetable import TimeTable
from metronome.common.specifications import TimeTableSpecifications

from metronome.common.constraint import Constraints

from typing import List

import math

class Node:
  
  def __init__(self, timeslot_id : int, venue_id : int, parent: 'Node' ,is_fundamental_root : bool = False):
    
    # Markers/Flags related to thhe node
    self.is_fundamental = is_fundamental_root
    self.is_feasible = True
    self.depth : int = parent.depth + 1 if parent is not None else -1
    
    # Variables realted to the actual problem.
    self.timeslot_id = timeslot_id
    self.venue_id = venue_id
    
    # References realted to the tree structure
    self.parent = parent
    self.children : List[Node] = []
    
    # Selection policy/characterization parameters
    self.number_of_violations = 0
    self.value = 0
    self.number_of_visits = 0
  
  def select(self, exploration_parameter : float = 2 ** 0.5):
    # Selection policy is implemented here.
    maximum_ucb = float('-inf')
    selected_child = None
    
    for child in self.children:
      if (child.is_feasible):
        if (child.number_of_visits == 0):
          return child
        ucb = child.value / child.number_of_visits + exploration_parameter * math.sqrt(math.log(self.parent.number_of_visits, math.e) / child.number_of_visits)
        if (ucb > maximum_ucb):
          maximum_ucb = ucb
          selected_child = child
    
    return selected_child  

  def expand(self, options : List[List[int]], timetable : TimeTable, constraints : Constraints) -> bool:
    # To expand the current node with iterate through
    # the options and check which ones are feasible.
    # Upon encountering a feasible child we add it to the
    # list of children.
    
    for option in options:
      # Adding option to timetable at this depth
      timetable.schedule_session(self.depth + 1, option[0], option[1])
      
      # Checking if the timetable is feasible
      if constraints.is_feasible(timetable, self.depth + 1):
        # Adding this child to the list of children
        self.children.append(Node(option[0], option[1], self))
    
    return len(self.children) > 0 # Indicates if the node was successfully expanded or not.
  
  def backpropagate_violations(self, violations : int):
    current_node = self
    while (not current_node.is_fundamental):
      current_node.number_of_violations = (current_node.number_of_violations * current_node.number_of_visits + violations) / (current_node.number_of_visits + 1)
      current_node.value = current_node.value_policy(current_node.number_of_violations)
      current_node = current_node.parent
      

  def value_policy(self, violations): # could just do self.number_of_violations
    return -violations
  
  def set_and_backpropagate_infeasiblility(self, penalty : float = 0.05):
    self.is_feasible = False # Oh no, this node is infeasible.
    
    # Hard backpropagation of infeasibility
    depth_of_infeasible_subtree : int = 1
    current_node = self.parent
    while current_node is not None:
      # Checking if the parent node has any feasible children
      print(f"At depth: {current_node.depth}")
      has_feasible_children : bool = False
      for child in current_node.children:
        if child.is_feasible:
          has_feasible_children = True
          break
      
      if not has_feasible_children:
        current_node.is_feasible = False
        depth_of_infeasible_subtree += 1
        current_node = current_node.parent
        continue
        
      break
        
    # SHOULD PROBABLY NOT SOFT-BACKPROPAGATE LIKE THIS ALL THE WAY TO THE 
    # FUNDAMENTAL ROOT.
    
    # Soft backpropagation of infeasibility: now all the 
    # seriously infeasible nodes have been marked
    # but the feasible nodes whose child/grandchild/... has been 
    # this infeasible subtree should be penalized.
    
    # Penalization is done via multiplying the number of violations into a fixed quantity.
    while (current_node is not None):
      
      # The penalization policy  below could possibly be improved.
      if (current_node.value < 0):
        current_node.value *= (1 + penalty * depth_of_infeasible_subtree)
      else:
        current_node.value -= penalty * depth_of_infeasible_subtree
      
      current_node = current_node.parent
      
  
  def is_leaf(self):
    return len(self.children) == 0 and self.is_feasible 
  
