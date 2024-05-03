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

# Getting the best timetable and its violations
best_timetable = genetic_optimizer.get_best_timetable()
violations = genetic_optimizer.violation_counter.calculate_weighted_violations(best_timetable)

# Printing out the best timetable and its violations
print("Best Timetable")
best_timetable.print()

print("\nViolations")
violation_labels = genetic_optimizer.violation_counter.get_violation_list()
#printing violations with formatting and justifications
for i, violation in enumerate(violations[1:]):
  print(f"{i:3d}. {violation_labels[i]:50s}: {violation:.4f}")
