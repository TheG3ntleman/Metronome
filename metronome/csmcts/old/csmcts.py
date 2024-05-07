from metronome.common.timetable import TimeTable
from metronome.common.reduced_problem import Problem
from metronome.common.specifications import TimeTableSpecifications

from metronome.csmcts.node import Node

from copy import deepcopy

class CSMCTS:
  
  def __init__(self, problem : Problem, timetable_specifications : TimeTableSpecifications, max_iterations : int  = 1000):
    self.problem = problem
    self.timetable_specifications = timetable_specifications
    self.max_iterations = max_iterations
    
    # MCTS Parameters
    self.simulations_per_epoch = 300 # Number of simulation to run in an epoch
    
    # The idea below can still be used to generate decent training example and not waste compute. However,
    # its implementation is complicated. For this reason, we skip it for now.
    # self.epoch_usable_depth = 5 # Depth of nodes during an epoch which can be added to the permanent tree.
  
  def run(self) -> TimeTable:
    
    # We have to add a root pseudo node.
    fundamental_root = Node(-1, -1, True)
    
    # State Variables
    current_timetable = TimeTable(self.timetable_specifications.sessions_table['size'])

    # We now run the main CS-MCTS Loop
    
    for simulation_i in range(self.max_iterations):
      # We will run for the maximum number of iterations here.
      
      # STEP -1 : Setting current node to fundamental root and setting depth to 0.
      current_node = fundamental_root # Current node will be fundamental root (not a copy but a reference).
      current_depth = 0
      
      # STEP 0  : Repeatedly applying the selection policy until we reach the leaf node (i.e a node has not been expanded.).
      while (not current_node.is_leaf()):
        selected_child = -1
        selected_child_average_violation_count = float('inf')
        for child in current_node.children:
          # We will select the child that has the lowest average violations
          # and is feasible.
          if (child.is_feasible and child.average_violations_per_child < selected_child_average_violation_count):
            selected_child = child
            selected_child_average_violation_count = child.average_violations_per_child
        current_node = selected_child # A suitable child has been selected.
      
      # STEP 1  : Expanding the current node.
      is_expansion_successful = current_node.expand(current_timetable, current_depth, self.problem.options[current_depth])

      # Handling failed expansion, occurs when node has no feasible children.
      if (not is_expansion_successful): # Main CS-MCTS Backtracking step
        current_node.backpropagate_infeasibility()
        
        # We have found a parent with feasible children, however, instead of going ahead and 
        # choosing it we will continue the loop. Even though this is ineffecient, it is
        # better as this parent may not be the optimal, moreover we must be at a leaf node.
        continue
        
      # STEP 2  : Running an epoch on this node, to get average violations per child node.
      # We are now at what was a leaf node but has now had its children expanded
      
      # Only this decision will be stored for the training of the neural network.
      self.mcts(current_node, current_timetable, current_depth)
      
      
           
  def mcts(self, original_node : Node, current_timetable : TimeTable, current_depth : int):
    # original_node has a node with it's children expanded.
    # We make a copy "current_node" of original_node which we will later delete (automatically, its sub-tree will also be 
    # deleted). Backpropgation will be done the fundamental root.
    
    """virtual_root = deepcopy(original_node)
    
    # We will now run the simulation for the epoch.
    for simulation_i in range(self.simulations_per_epoch):
      
      # STEP - -1: Checking if virtual root is infeasible.
      if virtual_root.is_feasible == False:
        break
      
      current_node = virtual_root
      # We will terminate after we have run the maximum simulations_per_epoch
      # or if it turns out that the entire branch is infeasible.
      
      # STEP - 0: Repeatedly applying the selection policy until we reach a leaf node
      while (not current_node.is_leaf()):
        current_node = current_node.optimally_select_child()
      
      # STEP 1 : Expanding the current node
      is_expansion_successful = current_node.expand(current_timetable, current_depth, self.problem.options[current_depth])
      
      # Handling failed expansion, occurs when node has no feasible children.
      if (not is_expansion_successful):
        current_node.backpropagate_infeasibility(root = virtual_root)
        continue"""
      
    
          
  
 
 