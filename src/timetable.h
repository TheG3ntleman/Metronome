#ifndef TIMETABLE_H
#define TIMETABLE_H

#include "common.h"

// PARTY DEFINITIONS
// COURSE NAME - PARTIES INVOLVED - TIME DURATION PER CLASS - NUMBER OF CLASSES


// struct of arrays instead of array of structs.
typedef struct {
  uint num_parties;
  char **party_names;

  uint num_jobs;
  char **job_names;
  uint **job_parties_needed;
  uint job_time_duration; //minutes
  uint job_occurances; // number of job reps a week.
} TimeTableSpecification;

#endif
