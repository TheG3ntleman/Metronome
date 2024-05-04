from metronome.common.timetable import TimeTable
from metronome.common.specifications import TimeTableSpecifications
from metronome.genetic.specifications import GeneticOptimizerSpecifications

import math
import random
from typing import List
import numpy as np

class MutationRateV1:
  
  def __init__(self, genetic_optimizer_specifications: GeneticOptimizerSpecifications) -> None:
    self.genetic_optimizer_specifications = genetic_optimizer_specifications
    self.phase = random.uniform(0, 2 * math.pi)
  
  def __call__(self, generation : int) -> float:
    
    if (generation > self.genetic_optimizer_specifications.terminal_region_generation):
      return self.genetic_optimizer_specifications.terminal_mutation_rate
    
    return min(self.genetic_optimizer_specifications.max_mutation_rate, 
               self.genetic_optimizer_specifications.ratio_of_chaos *
              math.exp(self.genetic_optimizer_specifications.spike_contrast *
                       math.sin(self.genetic_optimizer_specifications.periodic_frequency * generation + self.phase))) + \
            self.genetic_optimizer_specifications.min_mutation_rate

class MutationOperator:
    def __init__(self, time_table_specifications: TimeTableSpecifications) -> None:
      self.time_table_specifications = time_table_specifications
      
    def mutate(self, timetable : TimeTable, venue_mutation_rate : float, timeslot_mutation_rate : float) -> TimeTable:
        new_timetable = timetable.copy()
        
        # Iterating through every session to mutate
        for i in range(self.time_table_specifications.sessions_table['size']):
      
          if random.uniform(0, 1) < venue_mutation_rate:
            # Mutating the venue
            new_venue_index = random.randint(0, self.time_table_specifications.venue_table['size'] - 1)
            new_timetable.schedule_session(i, timetable.timetable[i]['timeslot_id'], self.time_table_specifications.venue_table['id'][new_venue_index])
          
          if random.uniform(0, 1) < timeslot_mutation_rate:
            # Mutating the timeslot
            new_timeslot_index = random.randint(0, self.time_table_specifications.timeslot_table['size'] - 1)
            new_timetable.schedule_session(i, self.time_table_specifications.timeslot_table['id'][new_timeslot_index], timetable.timetable[i]['venue_id'])
        
        return new_timetable
      
    def __call__(self, population : List[TimeTable], venue_mutation_rate : float, timeslot_mutation_rate : float) -> List[TimeTable]:
      new_population = []
      
      for timetable in population:
        new_population.append(self.mutate(timetable, venue_mutation_rate, timeslot_mutation_rate))
        
      return new_population