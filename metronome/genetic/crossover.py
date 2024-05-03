from metronome.common.timetable import TimeTable
from metronome.genetic.specifications import GeneticOptimizerSpecifications
import random

from typing import List

class CrossoverOperatorV1:
  
  def __init__(self, genetic_optimizer_specifications: GeneticOptimizerSpecifications, splits : List[float]) -> None:
    self.genetic_optimizer_specifications = genetic_optimizer_specifications
    self.splits = splits
  
  def __call__(self, population: List[TimeTable], violations_per_timetable : float) -> List[TimeTable]:
    new_population = []
    
    # Asserting that parent window is larger than primary parent window
    assert self.genetic_optimizer_specifications.parent_window > self.genetic_optimizer_specifications.primary_parent_window, "Parent window should be larger than primary parent window"
    
    # Finding the top timetables for the parent window
    parent_window_indices = violations_per_timetable.argsort()[:int((self.genetic_optimizer_specifications.parent_window / 100) * self.genetic_optimizer_specifications.population_size)]    
    
    # Finding indices of primary parents
    primary_parent_indices = parent_window_indices[:int((self.genetic_optimizer_specifications.primary_parent_window / 100) * self.genetic_optimizer_specifications.population_size)]
    
    for i in range(self.genetic_optimizer_specifications.population_size):
      # Choosing parents
      parent_a_i = random.choice(primary_parent_indices)
      parent_b_i = random.choice([i for i in parent_window_indices if i != parent_a_i])
      
      # Choosing parents c which is in parent_window_indices but now in primary_parent_indices
      parent_c_i = random.choice(parent_window_indices[int((self.genetic_optimizer_specifications.primary_parent_window / 100) * self.genetic_optimizer_specifications.population_size):])
      
      # Actually fetching the parents
      parent_a = population[parent_a_i]
      parent_b = population[parent_b_i]
      parent_c = population[parent_c_i]
      
      # Making a new empty time table
      new_timetable = TimeTable(population[i].size)
      
      # Iterating through sessions
      for j in range(new_timetable.size):
        # Randomly sampling a float between 0 and 1 (uniformly)
        toss = random.uniform(0, 1)
        
        if (toss > 0 and toss <= self.splits[0]):
          # We choose the first parent
          new_timetable.timetable[j] = parent_a.timetable[j]
        elif (toss > self.splits[0] and toss <= self.splits[1]):
          # We choose the second parent
          new_timetable.timetable[j] = parent_b.timetable[j]
        else:
          # We choose the third parent
          new_timetable.timetable[j] = parent_c.timetable[j]
        
      # Adding this timetable to the new population
      new_population.append(new_timetable)
    
    return new_population
  