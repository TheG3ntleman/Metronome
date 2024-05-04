from metronome.common.timetable import TimeTable
from metronome.genetic.specifications import GeneticOptimizerSpecifications
import random

from typing import List
import concurrent.futures
import numpy as np
"""
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
    
    return new_population"""


# Vectorized by chatGPT
class CrossoverOperatorV1:
  
    def __init__(self, genetic_optimizer_specifications: GeneticOptimizerSpecifications, splits: List[float]) -> None:
        self.genetic_optimizer_specifications = genetic_optimizer_specifications
        self.splits = splits
    
    def __call__(self, population: List[TimeTable], violations_per_timetable: np.ndarray) -> List[TimeTable]:
        new_population = []

        # Asserting that parent window is larger than primary parent window
        assert self.genetic_optimizer_specifications.parent_window > self.genetic_optimizer_specifications.primary_parent_window, "Parent window should be larger than primary parent window"

        # Finding the top timetables for the parent window
        parent_window_indices = np.argsort(violations_per_timetable)[:int((self.genetic_optimizer_specifications.parent_window / 100) * self.genetic_optimizer_specifications.population_size)]

        # Finding indices of primary parents
        primary_parent_indices = parent_window_indices[:int((self.genetic_optimizer_specifications.primary_parent_window / 100) * self.genetic_optimizer_specifications.population_size)]

        for i in range(self.genetic_optimizer_specifications.population_size):
            # Choosing parents
            parent_a_i = int(np.random.choice(primary_parent_indices, size=1))
            parent_b_i = int(np.random.choice(np.setdiff1d(parent_window_indices, parent_a_i), size=1))
            parent_c_i = int(np.random.choice(np.setdiff1d(parent_window_indices, primary_parent_indices), size=1))

            # Actually fetching the parents
            parent_a = population[parent_a_i].timetable
            parent_b = population[parent_b_i].timetable
            parent_c = population[parent_c_i].timetable

            # Making a new empty time table
            new_timetable = TimeTable(len(parent_a))

            # Iterating through sessions
            tosses = np.random.uniform(0, 1, size=new_timetable.size)

            # Assigning parents based on tosses
            for j in range(new_timetable.size):
                toss = tosses[j]
                if toss <= self.splits[0]:
                    new_timetable.timetable[j] = parent_a[j]
                elif toss <= self.splits[1]:
                    new_timetable.timetable[j] = parent_b[j]
                else:
                    new_timetable.timetable[j] = parent_c[j]

            # Adding this timetable to the new population
            new_population.append(new_timetable)

        return new_population