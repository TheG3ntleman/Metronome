from metronome import TimeTable
from metronome.genetic_optimizer import GeneticOptimizerSpecifications
from metronome.genetic_optimizer import ScalerGeneticOptimizer
from metronome.common import TimeTableSpecifications

sample_time_table_specifications = TimeTableSpecifications()
genetic_optimizer_specifications = GeneticOptimizerSpecifications()

scaler_genetic_optimizer = ScalerGeneticOptimizer(sample_time_table_specifications, genetic_optimizer_specifications)

# Running genetic optimizer 
scaler_genetic_optimizer.optimize()

# Plotting minimum violations and average violations in two seperate plots
scaler_genetic_optimizer.plot_violations()