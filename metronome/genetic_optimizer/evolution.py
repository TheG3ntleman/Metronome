from metronome.timetable import TimeTable
from matplotlib import pyplot as plt
from .violations import Violations
from tqdm import tqdm
import random
import numpy as np
import math


class GeneticOptimizerSpecifications:
  
  def __init__(self):
    self.number_of_generations = 150
    self.population_size = 1000
    self.top_k = int(self.population_size / 70)
    self.crossover_rate = 0.5
    
    self.constant_mutation_rate = 0.01
    self.constant_mutation_region = 70
    
    self.minimum_mutation_rate = 0.01
    self.maximum_mutation_rate = 0.5
    
    self.ratio_of_chaos = 0.001
    self.spike_constrast = 8.5
    
    self.periodic_phase = 6
    self.periodic_frequency = 0.7
    

class ScalerGeneticOptimizer:
    
  def __init__(self, time_table_specifications, genetic_optimizer_specifications : GeneticOptimizerSpecifications) -> None:
    # specifications and constraints
    self.time_table_specifications = time_table_specifications
    self.genetic_optimizer_specifications = genetic_optimizer_specifications
        
    # Other related objects
    self.violation_counter = Violations(time_table_specifications)
    self.most_fit_timetable_id = 0
    
    # Control Variables
    self.initialized = False
    
    # State Variables
    self.population = None
    self.current_mutation_rate = 0
    
    # Debug/Recording/Convergence Testing variables
    self.average_violations = []
    self.minimum_violations = []
    self.compartmental_violations = np.zeros((self.genetic_optimizer_specifications.number_of_generations, self.violation_counter.get_number_of_violations()))
  
  def initialize(self) -> None:
    # Initializing Population Randomly.
    self.population = []
    for timetable_id in range(self.genetic_optimizer_specifications.population_size):
      timetable = TimeTable(self.time_table_specifications.number_of_sessions)
      timetable.randomly_initialize(self.time_table_specifications)
      self.population.append(timetable)
    
    self.initialized = True
  
  def get_mutation_rate(self, generation):
    if generation < self.genetic_optimizer_specifications.constant_mutation_region:
      return  min(self.genetic_optimizer_specifications.ratio_of_chaos * math.exp(self.genetic_optimizer_specifications.spike_constrast * math.sin(self.genetic_optimizer_specifications.periodic_frequency * generation + self.genetic_optimizer_specifications.periodic_phase)), self.genetic_optimizer_specifications.maximum_mutation_rate) +  self.genetic_optimizer_specifications.minimum_mutation_rate
    else:
      return self.genetic_optimizer_specifications.constant_mutation_rate
  
  def optimize(self) -> None:
    if not self.initialized:
      self.initialize()
    
    # Evolutionary Optimization
    for generation in tqdm(range(self.genetic_optimizer_specifications.number_of_generations), desc = "Running Scalable Genetic Optimizer", total = self.genetic_optimizer_specifications.number_of_generations):
      # Violation (negative fitness) Computation
      violations = []
      
      average_compartmental_violations = np.zeros(self.violation_counter.get_number_of_violations())
      for timetable in self.population:
        violations_per_table = self.violation_counter.calculate_violations(timetable)
        average_compartmental_violations += violations_per_table[1:]
        violations.append(violations_per_table[0])
      self.compartmental_violations[generation] = average_compartmental_violations / self.genetic_optimizer_specifications.population_size
        
      #print("Len Violations:", len(violations))
      #print("Violations:", violations)
      # Appending average violations
      average_violations = 0
      for i in range(len(violations)):
        average_violations += violations[i]
      self.average_violations.append(average_violations / len(violations))
      
      # Appending minimum violations
      self.minimum_violations.append(min(violations))
      
      # Selecting top k individuals

      # Convert top_k to integer
      top_k = int(self.genetic_optimizer_specifications.top_k)

      # Selecting top k individuals
      parents = sorted(range(len(violations)), key=lambda i: violations[i])[:top_k]
      self.most_fit_timetable_id = parents[0]
      print("Parents of generation", generation + 1, ":", parents, " | Most fit timetable:", self.most_fit_timetable_id)



      # Crossover
      new_population = []
      for timetable_id in range(self.genetic_optimizer_specifications.population_size):
        parent1_id = parents[timetable_id % int(self.genetic_optimizer_specifications.top_k)]
        parent2_id = parents[(timetable_id + 1) % int(self.genetic_optimizer_specifications.top_k)]
        
        # Make sure parent1 and parent2 are different
        while parent1_id == parent2_id:
          parent2_id = (parent2_id + 1) % self.genetic_optimizer_specifications.top_k
          
        parent1 = self.population[parent1_id]
        parent2 = self.population[parent2_id]
        
        child = TimeTable(self.time_table_specifications.number_of_sessions)
        for session_id in range(self.time_table_specifications.number_of_sessions):
          uniform_sample = random.uniform(0, 1)
          if uniform_sample <= self.genetic_optimizer_specifications.crossover_rate:
            child.schedule_session(session_id, parent1.get_session_info(session_id)["timeslot_id"], parent1.get_session_info(session_id)["venue_id"])
          else:
            child.schedule_session(session_id, parent2.get_session_info(session_id)["timeslot_id"], parent2.get_session_info(session_id)["venue_id"])
        
        # If number of violations of child is greater than parents then choose the parent with minimum violations or randomly if both have same violations
        if self.violation_counter.calculate_violations(child)[0] > min(violations[parent1_id], violations[parent2_id]):
          if violations[parent1_id] < violations[parent2_id]:
            child = parent1
          elif violations[parent1_id] > violations[parent2_id]:
            child = parent2
          else:
            if random.uniform(0, 1) < 0.5:
              child = parent1
            else:
              child = parent2
        
        new_population.append(child)
      
      # Mutating the population
      self.current_mutation_rate = self.get_mutation_rate(generation)
      print("Mutation Rate:", self.current_mutation_rate)
      for timetable_id in range(self.genetic_optimizer_specifications.population_size):
        timetable = new_population[timetable_id]
        for session_id in range(self.time_table_specifications.number_of_sessions):
          uniform_sample = random.uniform(0, 1)
          if uniform_sample <= self.current_mutation_rate:
            timetable.schedule_session(session_id, 
                                        self.time_table_specifications.time_slot_ids[random.randint(0, self.time_table_specifications.number_of_time_slots - 1)], 
                                        self.time_table_specifications.venue_ids[random.randint(0, self.time_table_specifications.number_of_venues - 1)])
      
      # Setting the new population
      self.population = new_population
  
    # Converting compartmental violations to matrix of appropriate size
    #self.compartmental_violations = self.compartmental_violations.T
    self.average_violations  = np.array(self.average_violations)
    self.minimum_violations = np.array(self.minimum_violations)

  
  def plot_violations(self) -> None:
    print("SUMMARY:")
    print("\tNumber of Generations:", self.genetic_optimizer_specifications.number_of_generations)
    print("\tPopulation Size:", self.genetic_optimizer_specifications.population_size)
    print("\tMinimum Violations:", self.minimum_violations[-1])
    print("\tIndex of most fit timetable:", self.most_fit_timetable_id)
    
    plt.plot(self.average_violations)
    plt.title("Average Violations")
    plt.show()
    
    plt.plot(self.minimum_violations)
    plt.title("Minimum Violations")
    plt.show()
    
    # Plotting compartmental violations
    # Define curve names
    curve_names = self.violation_counter.get_violation_list()
    for i in range(self.violation_counter.get_number_of_violations()):
      plt.plot(self.compartmental_violations[:, i], label = curve_names[i])
    plt.legend()
    plt.title("Compartmental Violations")
    plt.show()
    

  
  def save_violations(self, path) -> None:
    # Save arrays as formatted text
    np.savetxt(path + "average_violations.txt", self.average_violations)
    np.savetxt(path + "minimum_violations.txt", self.minimum_violations)
    np.savetxt(path + "compartmental_violations.txt", self.compartmental_violations)
  
  def get_most_fit_timetable(self):
    return self.population[self.most_fit_timetable_id]
    
class DifferentialEvolution:
  
  def __init__(self, time_table_specifications, genetic_optimizer_specifications : GeneticOptimizerSpecifications) -> None:
    self.time_table_specifications = time_table_specifications
    self.genetic_optimizer_specifications = genetic_optimizer_specifications
    
    
  
  
  
