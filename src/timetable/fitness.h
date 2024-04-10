#ifndef FITNESS_H
#define FITNESS_H

#include "../common/config.h"
#include "../common/specifications.h"
#include "../common/utils.h"
#include "population.h"

void computeFitnesses(Population *population,
                      TimeTableSpecifications *specifications,
                      numeric *soft_fitness, numeric *hard_fitness);

numeric computeHardConstraint_CapacityConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);

numeric computeHardConstraint_VenueConflictConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);

numeric computeHardConstraint_VenueTypeConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);

numeric computeHardConstraint_PartyDuplicateConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);

numeric computeHardConstraint_sufficient_timeslotConstraint(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);

numeric computeSoftConstraint_studenttraveltime(Population *population,
                                        TimeTableSpecifications *specifications,
                                        uint timetable_index);

numeric computeSoftConstraint_maximize_chunking(Population *population,
                                        TimeTableSpecifications *specifications,
                                        uint timetable_index);


numeric computeSoftConstraint_room_utilization(Population *population,
                                       TimeTableSpecifications *specifications,
                                       uint timetable_index);

numeric computeSoftConstraint_avoid_early_late_session(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);


numeric computeSoftConstraint_room_capacity_utilization(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);


numeric computeSoftConstraint_common_timeslot_empty(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);



numeric computeSoftConstraint_backtoback_teacher_class(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);


numeric computeSoftConstraint_samecourse_session(
    Population *population, TimeTableSpecifications *specifications,
    uint timetable_index);


numeric computeSoftConstraint_lab_after_lecture(Population *population,
                                        TimeTableSpecifications *specifications,
                                        uint timetable_index);


numeric computeSoftConstraint_even_distribution(Population *population,
                                        TimeTableSpecifications *specifications,
                                        uint timetable_index);






#endif
