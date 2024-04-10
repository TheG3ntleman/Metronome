#include "problem.h"
#include "../common/constraint.h"
#include "src/common/specifications.h"
#include "src/common/utils.h"
#include "src/timetable/fitness.h"
#include "src/timetable/population.h"
#include <stdio.h>
#include <sys/types.h>
typedef numeric (*GA_Constraint)(Population *population,
                                 TimeTableSpecifications *specs,
                                 uint timetable_index);

typedef uint (*MCTS_Constraint)(TimeTableTuple *timetable,
                                TimeTableSpecifications *specs, uint depth);

#define path_mcts "/Users/charanganesh/Desktop/Projects/Metronome/mcts.csv"
#define path_GA "/Users/charanganesh/Desktop/Projects/Metronome/GA.csv"
const char *party_name_bank[7] = {"CM", "CSE", "AI", "ECM", "NT", "ME", "MT"};

/*Problem *make_problem() {
  // Allocating memory for the problem
  Problem *problem = (Problem *)malloc(sizeof(Problem));

  // Setting the number of parties
  problem->p_size =
      N_MINIMUM_PARTIES + rand() % (N_MAXIMUM_PARTIES - N_MINIMUM_PARTIES + 1);

  // Allocating memory for all party arrays accoring to specified size
  problem->p_party_id = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_strength = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_max_hours = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_party_type = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_preferred_start_time =
      (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_preferred_end_time =
      (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_max_back_to_back = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_preferred_max_hours =
      (uint *)malloc(sizeof(uint) * problem->p_size);

  // Set random values
  for (uint i = 0; i < problem->p_size; i++) {
    problem->p_party_id[i] = i;
    problem->p_strength[i] = rand() % 100 + 1;
    problem->p_max_hours[i] = 7;
    problem->p_party_type[i] = rand() % 2;
    problem->p_preferred_start_time[i] = rand() % 3;
    problem->p_preferred_end_time[i] = rand() % 4 + 4;
    problem->p_max_back_to_back[i] = rand() % 5;
    problem->p_preferred_max_hours[i] = rand() % 5 + 1;
  }

  // Randomly making party names
  problem->p_names = (char **)malloc(sizeof(char *) * problem->p_size);
  uint name_counters[7] = {0, 0, 0, 0, 0, 0, 0};
  for (uint i = 0; i < problem->p_size; i++) {
    problem->p_names[i] = (char *)malloc(sizeof(char) * 4);

    // Randomly selecting a party name from the bank
    uint name_index = rand() % 7;

    strcpy(problem->p_names[i], party_name_bank[name_index]);
    if (problem->p_names[i][2] == '\0') problem->p_names[i][2] = ' ';
    problem->p_names[i][3] = '0' + name_counters[name_index]++;

    }

    // Time slots
    problem->t_size = 35;
    problem->t_timeslot_id = (uint *)malloc(sizeof(uint) * problem->t_size);
    problem->t_day = (uint *)malloc(sizeof(uint) * problem->t_size);

    for (uint i = 0; i < problem->t_size; i++) {
        problem->t_timeslot_id[i] = i;
        problem->t_day[i] = i / 7; // Assuming 7 days, so each day has 3
timeslots
    }

    // Locality
    problem->l_size = 3; // Assuming 3 localities
    problem->l_locality_pair_index = (uint *)malloc(sizeof(uint) *
problem->l_size); problem->l_distance = (uint *)malloc(sizeof(uint) *
problem->l_size);

    for (uint i = 0; i < problem->l_size; i++) {
      for(uint j = i+1;j <problem->l_size;j++){
        problem->l_locality_pair_index[i] = locality_pair_to_index(i,j,
problem->l_size); problem->l_distance[problem->l_locality_pair_index[i]] =
rand() % 10 + 1; // Random distance between localities
      }

    }

    // Venue
    problem->v_size = 25; // Assuming 5 venues
    problem->v_venue_id = (uint *)malloc(sizeof(uint) * problem->v_size);
    problem->v_venue_type = (uint *)malloc(sizeof(uint) * problem->v_size);
    problem->v_capacity = (uint *)malloc(sizeof(uint) * problem->v_size);
    problem->v_locality = (uint *)malloc(sizeof(uint) * problem->v_size);
    uint max_venue_capacity = 0;

    for (uint i = 0; i < problem->v_size; i++) {
        problem->v_venue_id[i] = i;
        problem->v_venue_type[i] = rand() % 3; // Assuming 3 venue types
        problem->v_capacity[i] = rand()%70 + 30; // Assuming capacity of 30-100
for all venues problem->v_locality[i] = rand() % problem->l_size; // Assigning
random locality if (problem->v_capacity[i] > max_venue_capacity)
          max_venue_capacity = problem->v_capacity[i];
    }
    // Sessions
    problem->s_size = 50; // Assuming 3 sessions (too many sessions)
    problem->s_session_id = (uint *)malloc(sizeof(uint) * problem->s_size);
    problem->s_venue_type = (uint *)malloc(sizeof(uint) * problem->s_size);
    problem->s_duration = (uint *)malloc(sizeof(uint) * problem->s_size);
    problem->s_course = (uint *)malloc(sizeof(uint) * problem->s_size);

    for (uint i = 0; i < problem->s_size; i++) {
        problem->s_session_id[i] = i;
        problem->s_venue_type[i] = rand() % 3; // Assuming 3 venue types
        problem->s_duration[i] = rand() % 2 + 1; // Random session duration
between 1 to 3 hours problem->s_course[i] = rand() % 2; // Assuming 2 course
types
    }

    // Assignments
    // Setting number of assignments
    problem->a_size = 1.2 * problem->s_size; // Assuming 7 assignments
    problem->a_party_id = (uint *)malloc(sizeof(uint) * problem->a_size);
    problem->a_session_id = (uint *)malloc(sizeof(uint) * problem->a_size);
    problem->a_priority = (uint *)malloc(sizeof(uint) * problem->a_size);

    uint counter[problem->s_size];
    for (uint i = 0; i < problem->s_size;i++)
      counter[i] = 0;

    for (uint i = 0; i < problem->a_size;) {
        problem->a_party_id[i] = rand() % problem->p_size; // Assigning random
party ID problem->a_session_id[i] = rand() % problem->s_size; // Assigning
random session ID problem->a_priority[i] = 0; // Assuming priority is set to 0
for all assignments

        if (problem->p_strength[problem->a_party_id[i]] +
counter[problem->a_session_id[i]] < max_venue_capacity) { i++;
            counter[problem->a_session_id[i]] +=
problem->p_strength[problem->a_party_id[i]];
        }
    }

    // Randomly initializing constraint weights
    problem->constraint_weights = (float *)malloc(sizeof(float) * 15);
    for (uint i = 0; i < 6; i++) {
        problem->constraint_weights[i] = 1.0;
    }
    for (uint i = 6; i < 15; i++) {
        problem->constraint_weights[i] = 0;
    }

    return problem;
}

void print_problem(Problem *problem) {
    printf("Number of parties: %d\n", problem->p_size);

    // Printing party details
    for (uint i = 0; i < problem->p_size; i++) {
        printf("Party %d\n", i + 1);
        printf("\tName: %s\n", problem->p_names[i]);
        printf("\tStrength: %d\n", problem->p_strength[i]);
        printf("\tMax hours: %d\n", problem->p_max_hours[i]);
        printf("\tParty type: %d\n", problem->p_party_type[i]);
        printf("\tPreferred start time: %d\n",
problem->p_preferred_start_time[i]); printf("\tPreferred end time: %d\n",
problem->p_preferred_end_time[i]); printf("\tMax back to back: %d\n",
problem->p_max_back_to_back[i]); printf("\tPreferred max hours: %d\n",
problem->p_preferred_max_hours[i]); printf("\n");
    }

    // Printing timeslots
    printf("Number of timeslots: %d\n", problem->t_size);
    for (uint i = 0; i < problem->t_size; i++) {
        printf("Timeslot %d\n", i + 1);
        printf("\tTimeslot ID: %d\n", problem->t_timeslot_id[i]);
        printf("\tDay: %d\n", problem->t_day[i]);
        printf("\n");
    }

    // Printing localities
    printf("Number of localities: %d\n", problem->l_size);
    for (uint i = 0; i < problem->l_size; i++) {
        printf("Locality %d\n", i + 1);
        printf("\tLocality Pair Index: %d\n",
problem->l_locality_pair_index[i]); printf("\tDistance: %d\n",
problem->l_distance[i]); printf("\n");
    }

    // Printing venues
    printf("Number of venues: %d\n", problem->v_size);
    for (uint i = 0; i < problem->v_size; i++) {
        printf("Venue %d\n", i + 1);
        printf("\tVenue ID: %d\n", problem->v_venue_id[i]);
        printf("\tVenue Type: %d\n", problem->v_venue_type[i]);
        printf("\tCapacity: %d\n", problem->v_capacity[i]);
        printf("\tLocality: %d\n", problem->v_locality[i]);
        printf("\n");
    }

    // Printing sessions
    printf("Number of sessions: %d\n", problem->s_size);
    for (uint i = 0; i < problem->s_size; i++) {
        printf("Session %d\n", i + 1);
        printf("\tSession ID: %d\n", problem->s_session_id[i]);
        printf("\tVenue Type: %d\n", problem->s_venue_type[i]);
        printf("\tDuration: %d\n", problem->s_duration[i]);
        printf("\tCourse: %d\n", problem->s_course[i]);
        printf("\n");
    }

    // Printing assignments
    printf("Number of assignments: %d\n", problem->a_size);
    for (uint i = 0; i < problem->a_size; i++) {
        printf("Assignment %d\n", i + 1);
        printf("\tParty ID: %d\n", problem->a_party_id[i]);
        printf("\tSession ID: %d\n", problem->a_session_id[i]);
        printf("\tPriority: %d\n", problem->a_priority[i]);
        printf("\n");
    }
}


void free_problem(Problem *problem) {}*/

Problem *make_problem() {
  // Allocating memory for the problem
  Problem *problem = (Problem *)malloc(sizeof(Problem));

  // Setting the number of parties
  problem->p_size = 12; // Assuming 5 conferences

  // Allocating memory for all party arrays according to specified size
  problem->p_party_id = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_strength = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_max_hours = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_party_type = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_preferred_start_time =
      (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_preferred_end_time =
      (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_max_back_to_back = (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_preferred_max_hours =
      (uint *)malloc(sizeof(uint) * problem->p_size);
  problem->p_names = (char **)malloc(sizeof(char *) * problem->p_size);

  uint manual_party_strength[] = {70, 18, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  char *manual_party_names[] = {
      "ECM",         "CM",       "Gopinath", "Pooran",  "Ramakant", "veeraiya",
      "Jayaprakash", "ravibabu", "Mahesh",   "Praveen", "Pankaj",   "manvi"};

  // Set party data
  for (uint i = 0; i < problem->p_size; i++) {
    problem->p_party_id[i] = i;
    problem->p_strength[i] = manual_party_strength[i];
    problem->p_max_hours[i] = 2;
    problem->p_party_type[i] = 0;
    problem->p_preferred_start_time[i] = 1; // 9:30
    problem->p_preferred_end_time[i] = 6;   // 4:30
    problem->p_max_back_to_back[i] = 2;     // Maximum 2 sessions back to back
    problem->p_preferred_max_hours[i] =
        6; // Prefer conferences to last for 8 hours

    // Set party names
    problem->p_names[i] = strdup(manual_party_names[i]);
  }

  // Time slots
  problem->t_size = 35; // Assuming 35 time slots
  problem->t_timeslot_id = (uint *)malloc(sizeof(uint) * problem->t_size);
  problem->t_day = (uint *)malloc(sizeof(uint) * problem->t_size);

  // Assuming 5 days with 7 time slots each
  for (uint i = 0; i < problem->t_size; i++) {
    problem->t_timeslot_id[i] = i;
    problem->t_day[i] = i / 7;
  }

  // Locality
  problem->l_size = 3; // Assuming 2 localities
  problem->l_locality_pair_index =
      (uint *)malloc(sizeof(uint) * problem->l_size);
  problem->l_distance = (uint *)malloc(sizeof(uint) * problem->l_size);

  // Assuming distances between localities
  problem->l_locality_pair_index[0] = 0; // locality_i = 0, locality_j = 0
  problem->l_distance[0] = 0;
  problem->l_locality_pair_index[1] = 1; // locality_i = 0, locality_j = 1
  problem->l_distance[1] = 10;
  problem->l_locality_pair_index[2] = 2; // locality_i = 1, locality_j = 1
  problem->l_distance[2] = 0;

  // Venue
  problem->v_size = 15;
  problem->v_venue_id = (uint *)malloc(sizeof(uint) * problem->v_size);
  problem->v_venue_type = (uint *)malloc(sizeof(uint) * problem->v_size);
  problem->v_capacity = (uint *)malloc(sizeof(uint) * problem->v_size);
  problem->v_locality = (uint *)malloc(sizeof(uint) * problem->v_size);

  uint manual_venue_type[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1};
  uint manual_venue_capacity[] = {80, 80, 80,  80, 80, 80, 80, 80,
                                  80, 80, 150, 80, 80, 80, 150};
  uint manual_venue_locality[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1};

  // Set venue data
  for (uint i = 0; i < problem->v_size; i++) {
    problem->v_venue_id[i] = i;
    problem->v_venue_type[i] = manual_venue_type[i];
    problem->v_capacity[i] = manual_venue_capacity[i];
    problem->v_locality[i] = manual_venue_locality[i];
  }

  // Sessions
  problem->s_size = 30;
  problem->s_session_id = (uint *)malloc(sizeof(uint) * problem->s_size);
  problem->s_venue_type = (uint *)malloc(sizeof(uint) * problem->s_size);
  problem->s_duration = (uint *)malloc(sizeof(uint) * problem->s_size);
  problem->s_course = (uint *)malloc(sizeof(uint) * problem->s_size);

  uint manual_s_venue_type[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
                                2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0};
  uint manula_s_duration[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1,
                              2, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1};
  uint manula_s_course[] = {0, 1, 2, 3, 1, 0, 4, 2, 0, 4, 1, 3, 3, 3, 3,
                            4, 4, 5, 6, 6, 6, 7, 5, 6, 7, 7, 7, 7, 7, 5};

  // Set session data
  for (uint i = 0; i < problem->s_size; i++) {
    problem->s_session_id[i] = i;
    problem->s_venue_type[i] = manual_s_venue_type[i];
    problem->s_duration[i] = manula_s_duration[i];
    problem->s_course[i] = manula_s_course[i];
  }

  // Assignments
  problem->a_size = 64;
  problem->a_party_id = (uint *)malloc(sizeof(uint) * problem->a_size);
  problem->a_session_id = (uint *)malloc(sizeof(uint) * problem->a_size);
  problem->a_priority = (uint *)malloc(sizeof(uint) * problem->a_size);

  uint manual_a_session_id[] = {
      0,  0,  1,  1,  2,  2,  3,  3,  3,  4,  4,  5,  5,  6,  6,  7,
      7,  8,  8,  9,  9,  10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13,
      14, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 20, 21, 21, 22,
      22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29};

  uint manual_a_party_id[] = {0,  3, 0, 2, 0, 10, 0, 1, 5, 0, 2, 0, 3, 0, 4,  0,
                              10, 0, 3, 0, 4, 0,  2, 0, 1, 5, 0, 1, 8, 0, 1,  8,
                              0,  1, 5, 0, 4, 0,  4, 1, 6, 1, 9, 1, 1, 1, 11, 1,
                              6,  1, 9, 1, 7, 1,  7, 1, 7, 1, 7, 1, 7, 1, 6};

  // Set assignment data
  for (uint i = 0; i < problem->a_size; i++) {
    problem->a_party_id[i] = manual_a_party_id[i];
    problem->a_session_id[i] = manual_a_session_id[i];
    problem->a_priority[i] = 0;
  }

  // Randomly initializing constraint weights
  problem->constraint_weights = (float *)malloc(sizeof(float) * 15);
  for (uint i = 0; i < 6; i++) {
    problem->constraint_weights[i] = 1.0;
  }
  for (uint i = 6; i < 15; i++) {
    problem->constraint_weights[i] = 0;
  }

  return problem;
}

void print_problem(Problem *problem) {
  printf("Number of parties: %d\n", problem->p_size);

  // Printing party details
  for (uint i = 0; i < problem->p_size; i++) {
    printf("Party %d\n", i + 1);
    printf("\tName: %s\n", problem->p_names[i]);
    printf("\tStrength: %d\n", problem->p_strength[i]);
    printf("\tMax hours: %d\n", problem->p_max_hours[i]);
    printf("\tParty type: %d\n", problem->p_party_type[i]);
    printf("\tPreferred start time: %d\n", problem->p_preferred_start_time[i]);
    printf("\tPreferred end time: %d\n", problem->p_preferred_end_time[i]);
    printf("\tMax back to back: %d\n", problem->p_max_back_to_back[i]);
    printf("\tPreferred max hours: %d\n", problem->p_preferred_max_hours[i]);
    printf("\n");
  }

  // Printing timeslots
  printf("Number of timeslots: %d\n", problem->t_size);
  for (uint i = 0; i < problem->t_size; i++) {
    printf("Timeslot %d\n", i + 1);
    printf("\tTimeslot ID: %d\n", problem->t_timeslot_id[i]);
    printf("\tDay: %d\n", problem->t_day[i]);
    printf("\n");
  }

  // Printing localities
  printf("Number of localities: %d\n", problem->l_size);
  for (uint i = 0; i < problem->l_size; i++) {
    printf("Locality %d\n", i + 1);
    printf("\tLocality Pair Index: %d\n", problem->l_locality_pair_index[i]);
    printf("\tDistance: %d\n", problem->l_distance[i]);
    printf("\n");
  }

  // Printing venues
  printf("Number of venues: %d\n", problem->v_size);
  for (uint i = 0; i < problem->v_size; i++) {
    printf("Venue %d\n", i + 1);
    printf("\tVenue ID: %d\n", problem->v_venue_id[i]);
    printf("\tVenue Type: %d\n", problem->v_venue_type[i]);
    printf("\tCapacity: %d\n", problem->v_capacity[i]);
    printf("\tLocality: %d\n", problem->v_locality[i]);
    printf("\n");
  }

  // Printing sessions
  printf("Number of sessions: %d\n", problem->s_size);
  for (uint i = 0; i < problem->s_size; i++) {
    printf("Session %d\n", i + 1);
    printf("\tSession ID: %d\n", problem->s_session_id[i]);
    printf("\tVenue Type: %d\n", problem->s_venue_type[i]);
    printf("\tDuration: %d\n", problem->s_duration[i]);
    printf("\tCourse: %d\n", problem->s_course[i]);
    printf("\n");
  }

  // Printing assignments
  printf("Number of assignments: %d\n", problem->a_size);
  for (uint i = 0; i < problem->a_size; i++) {
    printf("Assignment %d\n", i + 1);
    printf("\tParty ID: %d\n", problem->a_party_id[i]);
    printf("\tSession ID: %d\n", problem->a_session_id[i]);
    printf("\tPriority: %d\n", problem->a_priority[i]);
    printf("\n");
  }
}

void free_problem(Problem *problem) {}

void wrapper_constraints_GA(Population *population,
                            TimeTableSpecifications *specs) {
  GA_Constraint functions[15] = {
      computeHardConstraint_CapacityConstraint,
      computeHardConstraint_VenueConflictConstraint,
      computeHardConstraint_VenueTypeConstraint,
      computeHardConstraint_PartyDuplicateConstraint,
      computeHardConstraint_sufficient_timeslotConstraint,
      computeSoftConstraint_studenttraveltime,
      computeSoftConstraint_maximize_chunking,
      computeSoftConstraint_room_utilization,
      computeSoftConstraint_avoid_early_late_session,
      computeSoftConstraint_room_capacity_utilization,
      computeSoftConstraint_common_timeslot_empty,
      computeSoftConstraint_backtoback_teacher_class,
      computeSoftConstraint_samecourse_session,
      computeSoftConstraint_lab_after_lecture,
      computeSoftConstraint_even_distribution};

  uint size = sizeof(functions) / sizeof(functions[0]);
  FILE *csvFile = fopen(path_GA, "a");
  if (csvFile == NULL) {
    printf("Failed to open the CSV file.\n");
    return;
  }
  for (uint i = 0; i < size; i++) {
    fprintf(csvFile, "Constraint: %u,", i);
    for (uint j = 0; j < population->n_timetables; j++) {
      numeric val = functions[i](population, specs, j);
      fprintf(csvFile, "%LF,", val);
      if(j == population->n_timetables-1){
        fprintf(csvFile, "\n");
      }
    }
  }
}

void wrapper_constraints_mcts(Population population,
                              TimeTableSpecifications *specs, uint depth) {
  for (int i = 0; i < population.n_timetables; i++) {
    TimeTableTuple temp = {i, i};

    MCTS_Constraint functions[16] = {hard_party_conflict,
                                     hard_repeated_tuple,
                                     hard_venue_capacity,
                                     hard_venue_type,
                                     hard_max_hours,
                                     hard_multi_timeslot,
                                     soft_travel_time,
                                     soft_chunking,
                                     soft_room_utilization,
                                     soft_extreme_time,
                                     soft_room_capacity_utilization,
                                     soft_common_timeslot_empty,
                                     soft_minimize_back_to_back,
                                     soft_repeated_course_session,
                                     soft_sessions_well_distributed,
                                     soft_lab_after_lecture};

    uint size = sizeof(functions) / sizeof(functions[0]);
    FILE *csvFile = fopen(path_mcts, "a");
    for (uint j = 0; j < size; j++) {
      uint val = functions[j](&temp, specs, depth);

      // uint val = Wrapper_Constraints_test(function_ptr, &temp, specs,
      // depth); path is defined at the top

      if (csvFile == NULL) {
        printf("Failed to open the CSV file.\n");
        return;
      }

      fprintf(csvFile, "%d,", val);
      fclose(csvFile);
    }
    if (i == population.n_timetables - 1) {
      fprintf(csvFile, "\n");
      fclose(csvFile);
    }
  }
}