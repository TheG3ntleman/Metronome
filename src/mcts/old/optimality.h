#ifndef OPTIMALITY_H
#define OPTIMALITY_H

#include "../timetable/specifications.h"
#include "agent.h"
#include "state_space_tree.h"

snumeric get_optimality(TimeTableEntry *timetable, uint depth,
                      TimeTableSpecifications *specifications);

// writing all soft constraint fitness functions so that they can be called in normalization.h
numeric soft_constraint_student_travel_time(TimeTableEntry *timetable,
                                            uint n_sessions, uint depth,
                                            TimeTableSpecifications *specs);

numeric soft_constraint_maximize_chunking(TimeTableEntry *timetable,
                                          uint n_sessions, uint depth,
                                          TimeTableSpecifications *specs);

numeric soft_constraint_room_utlisation(TimeTableEntry *timetable,
                                        uint n_sessions, uint depth,
                                        TimeTableSpecifications *specs);

numeric soft_constraint_avoid_early_time(TimeTableEntry *timetable,
                                         uint n_sessions, uint depth,
                                         TimeTableSpecifications *specs);

numeric soft_constraint_room_capacity_utilisation(TimeTableEntry *timetable,
                                          uint n_sessions, uint depth,
                                          TimeTableSpecifications *specs);

numeric soft_constraint_common_timeslot_empty(TimeTableEntry *timetable,
                                              uint n_sessions, uint depth,
                                              TimeTableSpecifications *specs);

numeric soft_constraint_minimize_backtoback_teacher_classes(TimeTableEntry *timetable,
                                              uint n_sessions, uint depth,
                                              TimeTableSpecifications *specs);

numeric soft_constraint_minimize_samecourse_sessions(TimeTableEntry *timetable,
                                             uint n_sessions, uint depth,
                                             TimeTableSpecifications *specs);

numeric soft_constraint_evenly_throughout_week(TimeTableEntry *timetable,
                                              uint n_sessions, uint depth,
                                              TimeTableSpecifications *specs);

#endif
