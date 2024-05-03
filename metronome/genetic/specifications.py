class GeneticOptimizerSpecifications:
  
  def __init__(self):
    
    # Number of timetables in the population
    self.population_size = 1000
    
    # Optimizer terminates after this
    self.max_generations = 10
    
    # Number of top timetables to select
    self.parent_window = 5 # Indicative of percentage of population
    self.primary_parent_window = 2 # Indicative of percentage of population
    
    # Mutation Rate related stuff
    self.terminal_mutation_rate = 0.01
    self.terminal_region_generation = 70
    
    self.min_mutation_rate = 0.01
    self.max_mutation_rate = 0.5
    
    self.ratio_of_chaos = 0.001
    self.spike_contrast = 8.5
    
    self.periodic_frequency = 0.1