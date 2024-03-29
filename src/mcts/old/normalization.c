
#include "normalization.h"

#define no_of_random_simulation 100
#define NUM_OF_CONSTRAINTS 9

snumeric normalize(StateSpaceTree *state_space_tree, Agent *agent,
                   TimeTableSpecifications *time_table_specifications,
                   MCTS_problem *problem, snumeric *maximum, snumeric *minimum) {

    // Define arrays to store the maximum and minimum values for each constraint
    numeric max_constraints[NUM_OF_CONSTRAINTS] = {0};
    numeric min_constraints[NUM_OF_CONSTRAINTS] = {0};

    for (uint i = 0; i < no_of_random_simulation; i++) {
        TimeTableEntry chosen_options[time_table_specifications->session_table->size];
        // generating a random timetable from given options
        for (uint j = 0; j < time_table_specifications->session_table->size; j++) {
            TimeTableEntry options[problem->n_options[agent->depth - 1]];
            for (uint k = 0; k < problem->n_options[agent->depth - 1]; k++) {
                options[k].timeslot = problem->problem[k]->timeslot;
                options[k].venue = problem->problem[k]->venue;
            }

            uint random_index = rand() % problem->n_options[agent->depth - 1];

            chosen_options[i].timeslot = options[random_index].timeslot;
            chosen_options[i].venue = options[random_index].venue;
        }

        // using a reward array to keep track of all the rewards from different constraints
        numeric reward[NUMBER_OF_CONSTRAINTS] = {0};

        // Update maximum and minimum arrays using soft constraint functions
        reward[0] = soft_constraint_student_travel_time(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        reward[1] = soft_constraint_maximize_chunking(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        reward[2] = soft_constraint_room_utlisation(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        reward[3] = soft_constraint_avoid_early_time(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        reward[4] = soft_constraint_room_capacity_utilisation(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        reward[5] = soft_constraint_common_timeslot_empty(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        reward[6] = soft_constraint_minimize_backtoback_teacher_classes(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        reward[7] = soft_constraint_minimize_samecourse_sessions(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        reward[8] = soft_constraint_evenly_throughout_week(chosen_options,time_table_specifications->session_table->size,
                                                                    time_table_specifications->session_table->size,
                                                                    problem->time_table_specifications);

        for (int j = 0; j < NUMBER_OF_CONSTRAINTS; j++) {
            if (reward[j] > max_constraints[j]) {
                max_constraints[j] = reward[j];
            }
            if (reward[j] < min_constraints[j]) {
                min_constraints[j] = reward[j];
            }
        }
        
    }

    // Copy the maximum and minimum values to the provided arrays
    for (int c = 0; c < NUM_OF_CONSTRAINTS; c++) {
        maximum[c] = max_constraints[c];
        minimum[c] = min_constraints[c];
    }
}


