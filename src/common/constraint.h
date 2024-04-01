#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "specifications.h"
#include "utils.h"

// HARD CONSTRAINTS

uint hard_party_conflict(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth);

// Needed to ensure that 2 different session don't happen at the same venue at
// the same time.
uint hard_repeated_tuple(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth);

uint hard_venue_capacity(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth);

uint hard_venue_type(TimeTableTuple *timetable, TimeTableSpecifications *specs,
                     uint depth);

uint hard_max_hours(TimeTableTuple *timetable, TimeTableSpecifications *specs,
                    uint depth);

uint hard_multi_timeslot(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth);

// SOFT CONSTRAINTS
uint soft_travel_time(TimeTableTuple *timetable,
                          TimeTableSpecifications *specs, uint depth);

uint soft_chunking(TimeTableTuple *timetable, TimeTableSpecifications *specs,
                   uint depth);

uint soft_room_utilization(TimeTableTuple *timetable,
                           TimeTableSpecifications *specs, uint depth);

uint soft_extreme_time(TimeTableTuple *timetable,
                       TimeTableSpecifications *specs, uint depth);

uint soft_room_capacity_utilization(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs, uint depth);

uint soft_common_timeslot_empty(TimeTableTuple *timetable,
                                TimeTableSpecifications *specs, uint depth);

uint soft_minimize_back_to_back(
    TimeTableTuple *timetable, TimeTableSpecifications *specs,
    uint depth); // Only for professors, sorry students :(


// Both the constraints below are done for days

uint soft_repeated_course_session(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth);

uint soft_sessions_well_distributed(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth);

uint soft_lab_after_lecture(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth);

#endif