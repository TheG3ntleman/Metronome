#include "metronome.h"
#include "testing/problem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int uint;

#define MINIMUM_PARTIES 10
#define MAXIMUM_PARTIES 1000
#define MINIMUM_PARTY_STRENGTH 10
#define MAXIMUM_PARTY_STRENGTH 100

int main() {

  srand(time(NULL));

  // Sample data for demonstration purposes
  Problem *problem = make_problem();
  print_problem(problem);

  /*

void solveTimeTablingProblem(
    // For party table
    size_t p_size, uint *p_id, uint *p_strength, uint *p_max_hours,
    uint *p_party_type, uint *p_preferred_start_time,
    uint *p_preferred_end_time, uint *p_preferred_back_to_back,
    uint *p_preferred_max_hours,

    // For venue table
    size_t v_size, uint *v_id, uint *v_type, uint *v_capacity, uint *v_locality,

    // For timeslot table
    size_t t_size, uint *t_id, uint *t_day,

    // For locality table
    size_t l_size, uint *l_id, uint *l_distance,

    // For session table
    size_t s_size, uint *s_id, uint *s_type, uint *s_duration, uint *s_course,

    // For assignments table
    size_t a_size, uint *a_party_id, uint *a_session_id, uint *a_priority,

    // For constraint weights
    float *constraint_weights)

  */

  // Solving the timetabling problem
  solveTimeTablingProblem(
      problem->p_size, problem->p_party_id, problem->p_strength,
      problem->p_max_hours, problem->p_party_type,
      problem->p_preferred_start_time, problem->p_preferred_end_time,
      problem->p_max_back_to_back, problem->p_preferred_max_hours,
      problem->v_size, problem->v_venue_id, problem->v_venue_type,
      problem->v_capacity, problem->v_locality, problem->t_size,
      problem->t_timeslot_id, problem->t_day, problem->l_size,
      problem->l_locality_pair_index, problem->l_distance, problem->s_size,
      problem->s_session_id, problem->s_venue_type, problem->s_duration,
      problem->s_course, problem->a_size, problem->a_party_id,
      problem->a_session_id, problem->a_priority, problem->constraint_weights);

  return 0;
}
