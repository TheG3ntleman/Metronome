#ifndef PROBLEM_H
#define PROBLEM_H

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

#define N_MINIMUM_PARTIES 10
#define N_MAXIMUM_PARTIES 20

#define N_MINIMUM_VENUES 25
#define N_MAXIMUM_VENUES 40

typedef struct {

  /*uint p_size = 4;
    uint p_party_id[4] = {0, 1, 2, 3};
    uint p_strength[4] = {10, 20, 30, 40};
    uint p_max_hours[4] = {5, 10, 15, 20};
    uint p_party_type[4] = {0, 1, 0, 1};
    uint p_preferred_start_time[4] = {0, 2, 0, 0};
    uint p_preferred_end_time[4] = {4, 5, 5, 3};
    uint p_max_back_to_back[4] = {7, 1, 7, 1};
    uint p_preferred_max_hours[4] = {7, 3, 7, 3};

    uint v_size = 5;
    uint v_venue_id[5] = {0, 1, 2, 3, 4};
    uint v_venue_type[5] = {0, 1, 2, 1, 0};
    uint v_capacity[5] = {60, 60, 60, 60, 60};
    uint v_locality[5] = {0, 0, 1, 1, 1};

    uint t_size = 4;
    uint t_timeslot_id[4] = {0, 1, 2, 3};
    uint t_day[4] = {0, 0, 1, 1};

    uint l_size = 2;
    uint l_locality_pair_index[1] = {0};
    uint l_distance[1] = {4};

    uint s_size = 3;
    uint s_session_id[3] = {0, 1, 2};
    uint s_venue_type[3] = {0, 0, 0};
    uint s_duration[3] = {2, 1, 1};
    uint s_course[3] = {0, 0, 1};

    uint a_size = 7;
    uint a_party_id[7] = {0, 1, 1, 2, 2, 2, 3};   //{0, 1, 2, 1, 2, 2, 3};
    uint a_session_id[7] = {0, 0, 1, 0, 1, 2, 2}; // {0, 0, 0, 1, 1, 2, 2};
    uint a_priority[7] = {0, 0, 0, 0, 0, 0, 0};

    float constraint_weights[16] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2,
                                    0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};*/

  // Party
  uint p_size;
  uint *p_party_id;
  uint *p_strength;
  uint *p_max_hours;
  uint *p_party_type;
  uint *p_preferred_start_time;
  uint *p_preferred_end_time;
  uint *p_max_back_to_back;
  uint *p_preferred_max_hours;

  // Venue
  uint v_size;
  uint *v_venue_id;
  uint *v_venue_type;
  uint *v_capacity;
  uint *v_locality;

  // Timeslot
  uint  t_size;
  uint *t_timeslot_id;
  uint *t_day;

  // Locality
  uint l_size;
  uint *l_locality_pair_index;
  uint *l_distance;

  // Session
  uint s_size;
  uint *s_session_id;
  uint *s_venue_type;
  uint *s_duration;
  uint *s_course;

  // Assignment
  uint a_size;
  uint *a_party_id;
  uint *a_session_id;
  uint *a_priority;

  float *constraint_weights;
  
  // NAMES

  // Party
  char **p_names;

  // Venue
  char **v_names;

  // Timeslot
  // char **timeslot_names;

  // Locality
  // char **locality_names;

  // Session
  char **session_names;

} Problem;

Problem *make_problem();
void print_problem(Problem *problem);
void free_problem(Problem *problem);

#endif