#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "specifications.h"
#include "utils.h"

// HARD CONSTRAINTS

uint hard_party_conflict_constraint(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs, uint depth);

// Needed to ensure that 2 different session don't happen at the same venue at the same
// time.
uint hard_repeated_tuple_constraint(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs, uint depth);

uint hard_venue_capacity_constraint(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs, uint depth);

uint hard_venue_type_constraint(TimeTableTuple *timetable,
                                TimeTableSpecifications *specs, uint depth);

uint hard_max_hours_constraint(TimeTableTuple *timetable,
                               TimeTableSpecifications *specs, uint depth);

uint hard_multi_timeslot_constraint(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs, uint depth);

// SOFT CONSTRAINTS

#endif