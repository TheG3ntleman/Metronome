from metronome.common.timetable import TimeTable
from metronome.common.specifications import TimeTableSpecifications
from metronome.common.problem import SyntheticProblemGenerator
from metronome.common.violations import Violations
from metronome.common.reduced_problem import MCTSTupleFrequencyProblem, Problem
from metronome.neural.string_gen import StringGenerator

from metronome.genetic.optimize import ScalerGeneticOptimizer
from metronome.genetic.specifications import GeneticOptimizerSpecifications

# Making empty specifications
empty_specifications = TimeTableSpecifications()
empty_specifications.venue_table['size']

# Generating a sample timetable specification
sample_time_table_specifications = SyntheticProblemGenerator.generate_feasible_specifications()
sample_time_table_specifications.print()

# Running the genetic optimizer on this specification
genetic_optimizer_specifications = GeneticOptimizerSpecifications()
genetic_optimizer = ScalerGeneticOptimizer(sample_time_table_specifications, genetic_optimizer_specifications)

genetic_optimizer.optimize()
#genetic_optimizer.plot_statistics()

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

# Printing out reduced problem from best population
reduced_problem = MCTSTupleFrequencyProblem(genetic_optimizer.population, 
                                            sample_time_table_specifications.timeslot_table['size'], 
                                            sample_time_table_specifications.venue_table['size'])
reduced_problem.print()

# Create an instance of Problem using the information from MCTSTupleFrequencyProblem
problem_instance = Problem(reduced_problem.number_of_sessions, reduced_problem.options)

# generating the string
string_generator = StringGenerator()

generated_string = string_generator.generate_pretty_string(sample_time_table_specifications, problem_instance, best_timetable, sample_time_table_specifications.sessions_table['size'] - 5)
print("Pretty String:\n", generated_string)

