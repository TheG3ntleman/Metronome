from metronome import TimeTable
from metronome.genetic_optimizer import GeneticOptimizerSpecifications
from metronome.genetic_optimizer import ScalerGeneticOptimizer
from metronome.common import TimeTableSpecifications
from metronome.mcts.problem import MCTSTupleFrequencyProblem, MCTSElementFrequencyProblem

sample_time_table_specifications = TimeTableSpecifications()
genetic_optimizer_specifications = GeneticOptimizerSpecifications()

scaler_genetic_optimizer = ScalerGeneticOptimizer(sample_time_table_specifications, genetic_optimizer_specifications)


manual_party_strength = [70, 18, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
manual_party_names = ["ECM", "CM", "Gopinath", "Pooran", "Ramakant", "veeraiya", "Jayaprakash", "ravibabu", "Mahesh", "Praveen", "Pankaj", "manvi"]
sample_time_table_specifications.number_of_parties = 12

# Set party data
for i in range(sample_time_table_specifications.number_of_parties):
    sample_time_table_specifications.party_ids.append(i)
    sample_time_table_specifications.party_strengths.append(manual_party_strength[i])
    sample_time_table_specifications.party_max_hours.append(2)
    sample_time_table_specifications.party_type.append(1)
    sample_time_table_specifications.party_preffered_start_time.append(1)  # 9:30
    sample_time_table_specifications.party_preffered_end_time.append(6)  # 4:30
    sample_time_table_specifications.party_preffered_back_to_back.append(2)  # Maximum 2 sessions back to back
    sample_time_table_specifications.party_preffered_max_hours.append(6)  # Prefer conferences to last for 8 hours
sample_time_table_specifications.party_type[0] = 0
sample_time_table_specifications.party_type[1] = 0

# Time slots
sample_time_table_specifications.number_of_time_slots = 35  # Assuming 35 time slots
sample_time_table_specifications.time_slot_ids = [i for i in range(sample_time_table_specifications.number_of_time_slots)]
sample_time_table_specifications.time_slot_days = [i // 7 for i in range(sample_time_table_specifications.number_of_time_slots)]

#locality table
sample_time_table_specifications.number_of_localities = 3
sample_time_table_specifications.locality_ids = [0, 1, 2, 3]  # locality_i = 0, locality_j = 0, locality_i = 0, locality_j = 1, locality_i = 1, locality_j = 1
sample_time_table_specifications.locality_distances = [0, 10, 10, 0]  # Distances between localities

#venue table
sample_time_table_specifications.number_of_venues = 15
manual_venue_type = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1]
manual_venue_capacity = [80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 150, 80, 80, 80, 150]
manual_venue_locality = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1]

for i in range(sample_time_table_specifications.number_of_venues):
    sample_time_table_specifications.venue_ids.append(i)
    sample_time_table_specifications.venue_types.append(manual_venue_type[i])
    sample_time_table_specifications.venue_capacities.append(manual_venue_capacity[i])
    sample_time_table_specifications.venue_locality.append(manual_venue_locality[i])

#sessions
sample_time_table_specifications.number_of_sessions = 30
manual_s_venue_type = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0]
manual_s_duration = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1]
manual_s_course = [0, 1, 2, 3, 1, 0, 4, 2, 0, 4, 1, 3, 3, 3, 3, 4, 4, 5, 6, 6, 6, 7, 5, 6, 7, 7, 7, 7, 7, 5]

# Set session data
for i in range(sample_time_table_specifications.number_of_sessions):
    sample_time_table_specifications.session_ids.append(i)
    sample_time_table_specifications.session_types.append(manual_s_venue_type[i])
    sample_time_table_specifications.session_durations.append(manual_s_duration[i])
    sample_time_table_specifications.session_courses.append(manual_s_course[i])   

# Assignments
sample_time_table_specifications.number_of_assignments = 63
manual_a_session_id = [0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 11,
                       12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 20,
                       21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29]

manual_a_party_id = [0, 3, 0, 2, 0, 10, 0, 1, 5, 0, 2, 0, 3, 0, 4, 0, 10, 0, 3, 0, 4, 0, 2, 0, 1, 5, 
                     0, 1, 8, 0, 1, 8, 0, 1, 5, 0, 4, 0, 4, 1, 6, 1, 9, 1, 1, 1, 11, 1, 6, 1, 9, 1, 7, 
                     1, 7, 1, 7, 1, 7, 1, 7, 1, 6]

# Set assignment data
for i in range(sample_time_table_specifications.number_of_assignments):
    sample_time_table_specifications.assignment_party_ids.append(manual_a_party_id[i])
    sample_time_table_specifications.assignment_session_ids.append(manual_a_session_id[i])
    sample_time_table_specifications.assignment_priorities.append(0) 

# Running genetic optimizer 
scaler_genetic_optimizer.optimize()

# Plotting minimum violations and average violations in two seperate plots
scaler_genetic_optimizer.plot_violations()
scaler_genetic_optimizer.save_violations("output/violations.txt")

most_fit_timetable = scaler_genetic_optimizer.get_most_fit_timetable()

print("\n\nMost fit timetable:")
most_fit_timetable.print()

# Printing out individual violation counts of all the constraints for the fit timetable
print("\n\nViolations:")

violations = scaler_genetic_optimizer.violation_counter.calculate_violations(most_fit_timetable)
for i in range(scaler_genetic_optimizer.violation_counter.get_number_of_violations()):
    print("\t", scaler_genetic_optimizer.violation_counter.get_violation_list()[i], ":", violations[i + 1])
    
# Converting the last generation to MCTS problems of both types 
mcts_tuple_frequency_problem = MCTSTupleFrequencyProblem(scaler_genetic_optimizer.population, sample_time_table_specifications.number_of_time_slots, sample_time_table_specifications.number_of_venues)
mcts_element_frequency_problem = MCTSElementFrequencyProblem(scaler_genetic_optimizer.population, sample_time_table_specifications.number_of_time_slots, sample_time_table_specifications.number_of_venues)

mcts_tuple_frequency_problem.print()
mcts_element_frequency_problem.print()
