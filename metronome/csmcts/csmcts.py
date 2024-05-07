from metronome.common.timetable import TimeTable
from metronome.common.reduced_problem import Problem
from metronome.common.specifications import TimeTableSpecifications
from metronome.common.constraint import Constraints

from metronome.csmcts.node import Node

from copy import deepcopy

class CSMCTS:
  
  def __init__(self, problem : Problem, timetable_specifications : TimeTableSpecifications, max_iterations : int  = 1000):
    # Variables related to the definition of the problem
    self.problem = problem
    self.specifications : TimeTableSpecifications = timetable_specifications
    self.constraints = Constraints(timetable_specifications)
    
    # Control Variables and hyper-parameters of the csmcts algorithm
    self.max_iterations = max_iterations
    self.number_of_simulations = 25
  
  def run(self):
    
    # We begin by defining the fundamental root at which no choice has yet been made. It 
    # denotes an empty timetable.
    fundamental_root : Node = Node(0, 0, None, True) 
    fundamental_root.number_of_visits = 1 # As we are visiting it.
    
    # Actually running the algorithm now
    for step in range(self.max_iterations):
      # STEP - 0 : Initializing this iteration
      current_node : Node = fundamental_root; #current_node.number_of_visits += 1 # as we are visiting it.
      depth : int  = 0
      timetable : TimeTable = TimeTable(self.specifications.sessions_table['size'])
      
      if not current_node.is_feasible:
        print("Fundamental root is infeasible.")
        exit()
      
      # STEP - 1 : Repeated Selection
      
      # In this stage we start from the root node and try to find a leaf node by 
      # following the selection policy. 
      print("Starting from fundamental root.")
      while (not current_node.is_leaf()):
        # We repeatedly apply the selection policy, choose the one with the highest 
        # value 
        current_node = current_node.select()
        print(f"Jumped to node at depth {current_node.depth} with options: {current_node.timeslot_id}, {current_node.venue_id}")
        # Updating the depth and timetable
        timetable.schedule_session(depth, current_node.timeslot_id, current_node.venue_id)
        depth += 1
      
      print(f"Found leaf node at depth {current_node.depth} with options: {current_node.timeslot_id}, {current_node.venue_id}")
        
      # STEP - 2: Checking if solution has been found
      if current_node.depth == self.specifications.sessions_table['size']:
        return timetable
      
      print(f"Solution not found ({current_node.depth} / {self.specifications.sessions_table['size']}).")
      
      # STEP - 3 : Expansion
      print("Beginning expansion.")
      # We now attempt to expand this leaf node.
      
      if not current_node.expand(self.problem[current_node.depth + 1], timetable, self.constraints):
        # Whoops this node is infeasible.
        current_node.set_and_backpropagate_infeasiblility()
        continue # Going back to the root node and retrying again.
        
      # At a node which has now been successfully expanded.
      print(f"Node successfully expanded with {len(current_node.children)} children.")
      
      # STEP - 4 : MCTS Simulation
      print(f"Running MCTS.")
      self.mcts(current_node, timetable) # This implicitly backpropagates and sets 
                                    # the value, average violations and visit counts of the immediate children.
      

      # STEP - 5 : Generatate training data
      # TODO: Will implement later.
    
  def mcts(self, node : Node, timetable : TimeTable):
    # Iterating accross the simulations
    for i in range(self.number_of_simulations):
      
      if not node.is_feasible:
        break
      
      print(f"Running simulation {i} / {self.number_of_simulations}")
      # STEP - 0 : Initializing this simulation
      print("Is root feasible: ", node.is_feasible) 
      current_node : Node = node
      current_depth : int = node.depth
      
      # STEP - 1 : Selection until arrival at leaf node
      while (not current_node.is_leaf()):
        current_node = current_node.select()
        print("Selecting a node, currently choosing: ", current_node.timeslot_id, current_node.venue_id, "at depth: ", current_node.depth)
        
        timetable.schedule_session(current_node.depth, current_node.timeslot_id, current_node.venue_id)
        current_depth += 1
      
      # STEP - 2 : Expansion
      print("At step 2")
      if not current_node.expand(self.problem[current_node.depth + 1], timetable, self.constraints):
        # Whoops this node is infeasible.
        print("No feasible child.")
        current_node.set_and_backpropagate_infeasiblility()
        continue
    
      # STEP - 3 : Rollouts
      print("At step 4")
      
    
      # STEP - 4 :  Backpropagation
      # Setting values (on negative violation policy) and violations
      # we then backpropagate them.
      
      # We are iterating through the children.
      for child in current_node.children:
        # Updating timetable to reflect the child
        timetable.schedule_session(child.depth, child.timeslot_id, child.venue_id)
        print(f"Printing timetable for child at depth: {child.depth}")
        timetable.print()
        number_of_violations = self.constraints.evaluate_soft_constraints(timetable, child.depth)
        
        #  Backpropagating violations
        current_node.backpropagate_violations(number_of_violations)
        child.number_of_visits += 1

    # Now cleaning up all other nodes.
    for child in node.children:
      if (child.is_feasible):
        child.children = []
          
        
        
        
        
        


       
	