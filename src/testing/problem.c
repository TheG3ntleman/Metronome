#include "problem.h"
#include <sys/types.h>

const char *party_name_bank[7] = {"CM", "CSE", "AI", "ECM", "NT", "ME", "MT"};

Problem *make_problem() {
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
        problem->t_day[i] = i / 7; // Assuming 7 days, so each day has 3 timeslots
    }

    // Locality
    problem->l_size = 3; // Assuming 3 localities
    problem->l_locality_pair_index = (uint *)malloc(sizeof(uint) * problem->l_size);
    problem->l_distance = (uint *)malloc(sizeof(uint) * problem->l_size);

    for (uint i = 0; i < problem->l_size; i++) {
      for(uint j = i+1;j <problem->l_size;j++){
        problem->l_locality_pair_index[i] = locality_pair_to_index(i,j, problem->l_size);
        problem->l_distance[problem->l_locality_pair_index[i]] = rand() % 10 + 1; // Random distance between localities
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
        problem->v_capacity[i] = rand()%70 + 30; // Assuming capacity of 30-100 for all venues
        problem->v_locality[i] = rand() % problem->l_size; // Assigning random locality
        if (problem->v_capacity[i] > max_venue_capacity) 
          max_venue_capacity = problem->v_capacity[i];
    }
    // Sessions
    problem->s_size = 50; // Assuming 3 sessions
    problem->s_session_id = (uint *)malloc(sizeof(uint) * problem->s_size);
    problem->s_venue_type = (uint *)malloc(sizeof(uint) * problem->s_size);
    problem->s_duration = (uint *)malloc(sizeof(uint) * problem->s_size);
    problem->s_course = (uint *)malloc(sizeof(uint) * problem->s_size);

    for (uint i = 0; i < problem->s_size; i++) {
        problem->s_session_id[i] = i;
        problem->s_venue_type[i] = rand() % 3; // Assuming 3 venue types
        problem->s_duration[i] = rand() % 2 + 1; // Random session duration between 1 to 3 hours
        problem->s_course[i] = rand() % 2; // Assuming 2 course types
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
        problem->a_party_id[i] = rand() % problem->p_size; // Assigning random party ID
        problem->a_session_id[i] = rand() % problem->s_size; // Assigning random session ID
        problem->a_priority[i] = 0; // Assuming priority is set to 0 for all assignments
        
        if (problem->p_strength[problem->a_party_id[i]] + counter[problem->a_session_id[i]] < max_venue_capacity) {
            i++;
            counter[problem->a_session_id[i]] += problem->p_strength[problem->a_party_id[i]];
        }
    }

    // Randomly initializing constraint weights
    problem->constraint_weights = (float *)malloc(sizeof(float) * 15);
    for (uint i = 0; i < 15; i++) {
        problem->constraint_weights[i] = 1.0;
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