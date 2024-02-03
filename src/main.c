#include "metronome.h"
#include "timetable/specifications.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int uint;

int main() {

  srand(time(NULL));

  // Sample data for demonstration purposes

  uint p_size = 4;
  uint p_party_id[4] = {0, 1, 2, 3};
  uint p_strength[4] = {10, 20, 30, 40};
  uint p_max_hours[4] = {5, 10, 15, 20};
  uint p_party_type[4] = {0, 1, 0, 1};
  uint p_preferred_start_time[4] = {0, 2, 0, 0};
  uint p_preferred_last_time[4] = {4, 5, 5, 3};
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
  uint a_priority_id[7] = {0, 0, 0, 0, 0, 0, 0};

  float constraint_weights[16] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2,
                                  0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};

  // Call the function with the prepared arguments
  solveTimeTablingProblem(p_size, p_party_id, p_strength, v_size, v_venue_id,
                          v_venue_type, v_capacity, v_number_available, s_size,
                          s_session_id, s_venue_type, s_duration, a_size,
                          a_party_id, a_session_id, a_priority);

  return 0;
}
