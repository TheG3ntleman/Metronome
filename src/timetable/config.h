#ifndef CONFIG_H
#define CONFIG_H


#include "utils.h"


// Evolutionary algorithm related specs
typedef struct {

  // Parameters related to selection and culling

  uint population_size;
  uint selection_size; // Parameter regarding how many organisms of current
                       // population are seleceted to generate new population.

  // Options for cluster based culling

  uint minimum_timetables_per_cluster;

  // The similarity value after which two timetables
  // are put in the same cluster without further computation.
  numeric similarity_threshold;

  // Parameters related to mutation operation
  numeric mutation_rate;

  // Options for selection based culling

  // Remember the norm used can be swapped.

  // Termination condition related options
  // uint minimum_generations;
  uint maximum_generations;
  // numeric minimum_soft_epsilon;
  // numeric minimum_hard_epsilon;

} GeneticSpecifications;

// ERROR CODES
#define ERR_SESSION_ID_OVERFLOW 1

// FITNESS RELATED OPTIONS

#define NUMBER_OF_DAYS 6
#define MAX_HOURS_PER_DAY 5

// Hard constraint controls
#define HARD_CAPACITY_CONSTRAINT
#define HARD_VENUE_CONFLICT
#define HARD_VENUE_TYPE
// #define HARD_MAX_SESSIONS
#define HARD_PARTY_DUPLICATE
#define HARD_SUFFECIENT_TIMESLOT

// Hard constraint controls
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

#define NUMBER_OF_CONSTRAINTS 16

#endif
