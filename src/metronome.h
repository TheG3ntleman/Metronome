#ifndef METRONOME_H
#define METRONOME_H

#include "common/config.h"
#include "timetable/evolution.h"
#include "mcts/monte_carlo_tree_search.h"
#include <stdlib.h>

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
    float *constraint_weights);


#endif
