from metronome.common.timetable import TimeTable
from metronome.common.problem import SyntheticProblemGenerator
from metronome.common.violations import Violations

from metronome.genetic.optimize import ScalerGeneticOptimizer
from metronome.genetic.specifications import GeneticOptimizerSpecifications

# Generating a sample timetable specification
sample_time_table_specifications = SyntheticProblemGenerator.generate_feasible_specifications()
sample_time_table_specifications.print()

# Running the genetic optimizer on this specification
genetic_optimizer_specifications = GeneticOptimizerSpecifications()
genetic_optimizer = ScalerGeneticOptimizer(sample_time_table_specifications, genetic_optimizer_specifications)

genetic_optimizer.optimize()
genetic_optimizer.plot_statistics()