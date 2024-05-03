from metronome.common.timetable import TimeTable
from metronome.common.violations import WeightedViolations 
from metronome.common.specifications import TimeTableSpecifications

from metronome.genetic.specifications import GeneticOptimizerSpecifications
from metronome.genetic.crossover import CrossoverOperatorV1
from metronome.genetic.mutation import MutationRateV1, MutationOperator

from typing import List

from matplotlib import pyplot as plt
import random
import numpy as np
import math

  
class ScalerGeneticOptimizer:
  
  def __init__(self, time_table_specifications: TimeTableSpecifications, genetic_optimizer_specifications: GeneticOptimizerSpecifications, printstep : int = 10) -> None:
    # Setting specifications
    self.time_table_specifications = time_table_specifications
    self.genetic_optimizer_specifications = genetic_optimizer_specifications
    
    # genetic operators
    self.crossover_operator = CrossoverOperatorV1(self.genetic_optimizer_specifications, [0.33, 0.66])
    self.mutation_rate_venue = MutationRateV1(self.genetic_optimizer_specifications)
    self.mutation_rate_timeslot = MutationRateV1(self.genetic_optimizer_specifications)
    self.mutation_operator = MutationOperator(self.time_table_specifications)
    
    # Other related objects
    self.violation_counter = WeightedViolations(self.time_table_specifications)
    
    # State variables
    self.population = None
    
    # IO/Debug variables
    self.printstep = printstep
    self.average_violations = []
    self.minimum_violations = []  
    self.compartmental_violations = np.zeros((self.genetic_optimizer_specifications.max_generations, 
                                              self.violation_counter.get_number_of_violations()))  
    
    
  def initialize_population(self, number_of_sessions : int, number_of_timetables : int) -> List[TimeTable]:
    population = []
    
    for _ in range(number_of_timetables):
      timetable = TimeTable(number_of_sessions)
      timetable.random(self.time_table_specifications)
      population.append(timetable)
      
    return population
  
  def evolve(self) -> None:
    
    for gen_i in range(self.genetic_optimizer_specifications.max_generations):
      # Iterating across generations.
      
      # STEP 0 - Calculate the violations of the population
      violations_per_timetable = np.zeros(self.genetic_optimizer_specifications.population_size)
      average_violation = 0
      minimum_violation = 0
      for i, timetable in enumerate(self.population):
        violations  = self.violation_counter.calculate_weighted_violations(timetable)
        
        # Appending average and min violations
        average_violation += violations[0]
        minimum_violation = min(minimum_violation, violations[0])
        
        # Adding compartmental violations
        self.compartmental_violations[gen_i] = violations[1:]
        
        # Adding violations to violation list.
        violations_per_timetable[i] = violations[0]
      
      average_violation /= self.genetic_optimizer_specifications.population_size
      
      # Appending average and min violations
      self.average_violations.append(average_violation)
      self.minimum_violations.append(minimum_violation)
        
      # STEP 1 - Selection and Crossover
      self.population = self.crossover_operator(self.population, violations_per_timetable)
      
      # STEP 2 - Mutation
      self.population = self.mutation_operator(self.population, self.mutation_rate_venue(gen_i), self.mutation_rate_timeslot(gen_i))
      
      if (gen_i % self.printstep == 0 or gen_i == self.genetic_optimizer_specifications.max_generations - 1):
        # Printing the average and min violations in a formatted and justified way.
        print(f"Generation {gen_i} - Average Violations: {np.mean(violations_per_timetable):5.2f} - Min Violations: {np.min(violations_per_timetable):5.2f}")
        #print(f"Generation {gen_i} - Average Violations: {np.mean(violations_per_timetable)} - Min Violations: {np.min(violations_per_timetable)}")      
  
  def optimize(self):
    self.population = self.initialize_population(self.time_table_specifications.sessions_table['size'], 
                                                 self.genetic_optimizer_specifications.population_size)
    self.evolve()
  
  def plot_statistics(self):
    
    # printing summary
    print("================================================")
    print("Summary:")
    print(f"\tNumber of Generations: {self.genetic_optimizer_specifications.max_generations}")
    print(f"\tPopulation Size: {self.genetic_optimizer_specifications.population_size}")
    print(f"\tAverage Violations: {self.average_violations[-1]}")
    print(f"\tMinimum Violations: {self.minimum_violations[-1]}")
    print("================================================")
     
    
    # Plotting average and minimum violations
    plt.plot(self.average_violations, label='Average Violations')
    plt.plot(self.minimum_violations, label='Minimum Violations')
    plt.legend()
    plt.show()
    
    # Plotting compartmental violations
    label_for_violations = self.violation_counter.get_violation_list()
    for i in range(self.compartmental_violations.shape[1]):
      plt.plot(self.compartmental_violations[:, i], label=label_for_violations[i])
    plt.legend()
    plt.show()
    