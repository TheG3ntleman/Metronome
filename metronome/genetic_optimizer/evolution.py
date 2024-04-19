from metronome.common import TimeTableSpecifications
from metronome.common import Constraints 

class GeneticOptimizerSpecifications:
  
  def __init__(self):
    self.population_size = 1000

class DifferentialEvolution:
  
  def __init__(self, time_table_specifications : TimeTableSpecifications, genetic_optimizer_specifications : GeneticOptimizerSpecifications) -> None:
    self.time_table_specifications = time_table_specifications
    self.genetic_optimizer_specifications = genetic_optimizer_specifications
    
    self.constraints = Constraints(time_table_specifications)
    
  
  
  