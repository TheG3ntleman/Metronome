#ifndef CONFIG_H
#define CONFIG_H

#define NUMBER_OF_DAYS 6
#define MAX_HOURS_PER_DAY 5

// Hard constraint specfications
#define HARD_CAPACITY_CONSTRAINT
#define HARD_VENUE_CONFLICT
#define HARD_VENUE_TYPE
#define HARD_MAX_SESSIONS
#define HARD_PARTY_DUPLICATE
#define HARD_SUFFECIENT_TIMESLOT
#define SOFT_STUDENT_TRAVELTIME
#define SOFT_MAXIMIZE_CHUNKING
#define SOFT_ROOM_UTILIZATION
#define SOFT_AVOID_EARLYLATE_TIME
#define SOFT_ROOM_CAPACITY_UTILIZATION
#define SOFT_COMMON_TIMSLOT_EMPTY
#define SOFT_MINIMIZE_BACKTOBACK_TEACHERCLASSES
#define SOFT_MINIMIZE_SAMECOURSE_SESSION
#define SOFT_LAB_AFTER_LECTURE
#define SOFT_SESSIONS_EVENLY_THROUGHOUT_WEEK

#include "utils.h"
#include <stdlib.h>

typedef struct {
	uint population_size;
	uint selection_size;
	uint min_timetable_per_cluster;
	numeric epsilon_soft;
	numeric epsilon_hard;
} GASpecfications;

// GASpecifications related functions
GASpecfications* makeGASpecifications();
void deleteGASpecfications(GASpecfications *specs);

#endif
