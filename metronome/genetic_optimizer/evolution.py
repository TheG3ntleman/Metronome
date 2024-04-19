from metronome.timetable import TimeTable
#from metronome.common import TimeTableSpecifications
from matplotlib import pyplot as plt
#from metronome.common import Constraints 
from .violations import Violations
from tqdm import tqdm
import random


class GeneticOptimizerSpecifications:
  
  def __init__(self):
    self.number_of_generations = 10
    self.population_size = 1000
    self.top_k = self.population_size / 100
    self.crossover_rate = 0.3
    self.mutation_rate = 0.01

class ScalerGeneticOptimizer:
    
  def __init__(self, time_table_specifications, genetic_optimizer_specifications : GeneticOptimizerSpecifications) -> None:
    # specifications and constraints
    self.time_table_specifications = time_table_specifications
    self.genetic_optimizer_specifications = genetic_optimizer_specifications
        
    # Other related objects
    self.violation_counter = Violations(time_table_specifications)
    
    # Control Variables
    self.initialized = False
    
    # State Variables
    self.population = None
    
    # Debug/Recording/Convergence Testing variables
    self.average_violations = []
    self.minimum_violations = []
  
  def initialize(self) -> None:
    # Initializing Population Randomly.
    self.population = []
    for timetable_id in range(self.genetic_optimizer_specifications.population_size):
      timetable = TimeTable(self.time_table_specifications.number_of_sessions)
      timetable.randomly_initialize(self.time_table_specifications)
      self.population.append(timetable)
    
    self.initialized = True
    
  def optimize(self) -> None:
    if not self.initialized:
      self.initialize()
    
    # Evolutionary Optimization
    for generation in tqdm(range(self.genetic_optimizer_specifications.number_of_generations), desc = "Running Scalable Genetic Optimizer", total = self.genetic_optimizer_specifications.number_of_generations):
      # Violation (negative fitness) Computation
      violations = [self.violation_counter.calculate_violations(timetable) for timetable in self.population]
      print("Len Violations:", len(violations))
      print("Violations:", violations)
      # Appending average violations
      average_violations = 0
      for i in range(len(violations)):
        average_violations += violations[i]
      self.average_violations.append(average_violations / len(violations))
      
      # Appending minimum violations
      self.minimum_violations.append(min(violations))
      
      # Selecting top k individuals
      parents = sorted(range(len(violations)), key = lambda sub: violations[sub])[-self.genetic_optimizer_specifications.top_k:]
      
      # Crossover
      new_population = []
      for timetable_id in range(self.genetic_optimizer_specifications.population_size):
        parent1_id = parents[timetable_id % self.genetic_optimizer_specifications.top_k]
        parent2_id = parents[(timetable_id + 1) % self.genetic_optimizer_specifications.top_k]
        
        # Make sure parent1 and parent2 are different
        while parent1_id == parent2_id:
          parent2_id = (parent2_id + 1) % self.genetic_optimizer_specifications.top_k
          
        parent1 = self.population[parent1_id]
        parent2 = self.population[parent2_id]
        
        child = TimeTable(self.time_table_specifications.number_of_sessions)
        for session_id in range(self.time_table_specifications.number_of_sessions):
          uniform_sample = random.runif(0, 1)
          if uniform_sample <= self.genetic_optimizer_specifications.crossover_rate:
            child.schedule_session(session_id, parent1.get_session_info(session_id)["timeslot_id"], parent1.get_session_info(session_id)["venue_id"])
          else:
            child.schedule_session(session_id, parent2.get_session_info(session_id)["timeslot_id"], parent2.get_session_info(session_id)["venue_id"])
        
        # If number of violations of child is greater than parents then choose the parent with minimum violations or randomly if both have same violations
        if self.violation_counter.calculate_violations(child) > min(violations[parent1_id], violations[parent2_id]):
          if violations[parent1_id] < violations[parent2_id]:
            child = parent1
          elif violations[parent1_id] > violations[parent2_id]:
            child = parent2
          else:
            if random.runif(0, 1) < 0.5:
              child = parent1
            else:
              child = parent2
        
        new_population.append(child)
      
      # Mutating the population
      for timetable_id in range(self.genetic_optimizer_specifications.population_size):
        timetable = new_population[timetable_id]
        for session_id in range(self.time_table_specifications.number_of_sessions):
          uniform_sample = random.runif(0, 1)
          if uniform_sample <= self.genetic_optimizer_specifications.mutation_rate:
            timetable.schedule_session(session_id, 
                                        self.time_table_specifications.time_slot_ids[random.randint(0, self.time_table_specifications.number_of_time_slots - 1)], 
                                        self.time_table_specifications.venue_ids[random.randint(0, self.time_table_specifications.number_of_venues - 1)])
      
      # Setting the new population
      self.population = new_population
  
  def plot_violations(self) -> None:
    plt.plot(self.average_violations)
    plt.title("Average Violations")
    plt.show()
    
    plt.plot(self.minimum_violations)
    plt.title("Minimum Violations")
    plt.show()
class DifferentialEvolution:
  
  def __init__(self, time_table_specifications, genetic_optimizer_specifications : GeneticOptimizerSpecifications) -> None:
    self.time_table_specifications = time_table_specifications
    self.genetic_optimizer_specifications = genetic_optimizer_specifications
    
    
  
  
  