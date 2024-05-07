from metronome.common.timetable import TimeTable
from metronome.common.violations import is_feasible, evaluate_soft_constraints

from typing import List

class Node:
  
  def __init__(self, timeslot_id : int, venue_id : int, parent: 'Node' ,is_fundamental_root : bool = False):
		# Node related attributes
    self.is_fundamental_root = is_fundamental_root
    self.is_feasible = True
    
    # Time Table related attributes
    self.timeslot_id = 0
    self.venue_id = 0
    
    # Parents and children
    self.parent = parent
    self.children : List[Node] = []

    # Actual MCTS related variables
    self.average_violations = 0
    self.visit_counts = 0
  
  def expand(self, timetable : TimeTable, depth : int, options : List[List[int]]) -> bool:
    # We will now check which of the options are feasible.
    # Upon encountering a feasible child we will add it.
    
    # STEP 0: Iterating over all the options
    for option in options:
      
      # Modiyfing the timetable to include 
      # the option we will test right now.
      timetable.schedule_session(depth, option['timeslot_id'], option['venue_id'])
      
      # Checking feasibility
      if is_feasible(timetable, depth + 1):
        # Feasible child, making a node instance for it.
        feasible_child = Node(option['timeslot_id'], option['venue_id'], self)
        # Adding the child to the children list
        self.children.append(feasible_child)
    
    # Checking if there are no feasible children
    if (len(self.children) == 0):
      # No feasible children, we will have to backtrack.
      return False
    return True
  
  def optimally_select_child(self) -> 'Node':
    # We must select a feasible child with the lowest average violations.
    # or if it is feasible and has a zero visit count.
    optimal_child = None
    optimal_child_average_violations = float('inf')
    for child in self.children:
      if (child.is_feasible):
        if (child.visit_counts == 0):
          return child
        if (child.average_violations < optimal_child_average_violations):
          optimal_child = child
          optimal_child_average_violations = child.average_violations
    
    if optimal_child is None:
      raise "No feasible child found. Very Weird."
    
    return optimal_child

  def backpropagate_violations(self, violations : float):
    pass
  
  def backpropagate_infeasibility(self, penalty : float = 100.0) -> int:
    # We backtrack to last feasible node.
    self.is_feasible = False
    depth_of_infeasible_subtree = 1
    current_node = self.parent # Jumping to parent
    
    while (True): # Sad that I had to do this.
      # Checking if the parent has any feasible children
      has_feasible_children = False
      for child in current_node.children:
        if (child.is_feasible):
          has_feasible_children = True
          break
      
      # This parent node also has no feasible children
      if (not has_feasible_children):
        # Checking if we have reached the fundamental root
        if (current_node.is_fundamental_root):
          #raise "No feasible timetable found.
          current_node.is_feasible = False
          print("No feasible time table found.")
          break

        current_node.is_feasible = False
        current_node = current_node.parent # Is it possible to DELETE THE INFEASIBLE SUB-TREE OUT OF EXISTANCE?
        depth_of_infeasible_subtree += 1
      else:
        break
    
    # We are now at a supposedly feasible node, however we must 
    # backpropagate the infeasibility as a average violation penalty.
    current_node.backpropagate_violations(penalty * depth_of_infeasible_subtree)
    return depth_of_infeasible_subtree    
  
  def is_leaf(self) -> bool:
    return len(self.children == 0) and self.is_feasible
  