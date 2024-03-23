#include "problem.h"

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

  // Randomly making party names
  problem->p_names = (char **)malloc(sizeof(char *) * problem->p_size);
  for (uint i = 0; i < problem->p_size; i++) {
    problem->p_names[i] = (char *)malloc(sizeof(char) * 4);

    // Randomly selecting a party name from the bank
    uint name_index = rand() % 7;
    uint name_counters[7] = {0, 0, 0, 0, 0, 0, 0};

    strcpy(problem->p_names[i], party_name_bank[name_index]);
    problem->p_names[i][3] = '0' + name_counters[name_index]++;
    
    }
}

void print_problem(Problem *problem) {
  printf("Number of parties: %d\n", problem->p_size);
  for (uint i = 0; i < problem->p_size; i++) {
    printf("Party %d\n", i + 1);
    printf("Name: %s\n", problem->p_names[i]);
    printf("Strength: %d\n", problem->p_strength[i]);
    printf("Max hours: %d\n", problem->p_max_hours[i]);
    printf("Party type: %d\n", problem->p_party_type[i]);
    printf("Preferred start time: %d\n", problem->p_preferred_start_time[i]);
    printf("Preferred end time: %d\n", problem->p_preferred_end_time[i]);
    printf("Max back to back: %d\n", problem->p_max_back_to_back[i]);
    printf("Preferred max hours: %d\n", problem->p_preferred_max_hours[i]);
  }
}

void free_problem(Problem *problem) {}