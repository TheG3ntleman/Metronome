#include "fitness.h"
#include "src/timetable/population.h"

void computeFitnesses(Population *population,
                      TimeTableSpecifications *specifications,
                      numeric *soft_fitness, numeric *hard_fitness) {

  printf("Calculating fitness.\n");

  float average_hard_party_conflict = 0;
  float average_hard_repeated_tuple = 0;
  float average_hard_venue_capacity = 0;
  float average_hard_venue_type = 0;
  float average_hard_max_hours = 0;
  float average_hard_multi_timeslot = 0;

  float average_soft_travel_time = 0;
  float average_soft_chunking = 0;
  float average_soft_room_utilization = 0;
  float average_soft_extreme_time = 0;
  float average_soft_room_capacity_utilization = 0;
  // numeric average_soft_common_timeslot_empty = 0;
  // numeric average_soft_minimize_back_to_back = 0;
  // numeric average_soft_repeated_course_session = 0;
  float average_soft_sessions_well_distributed = 0;
  // numeric average_soft_lab_after_lecture = 0;

  // Setting everything in soft and hard fitness input arrays to 0 (initializing them)
  for (uint i = 0; i < population->n_timetables; i++) {
    soft_fitness[i] = 0;
    hard_fitness[i] = 0;
  }

  for (uint timetable_index = 0; timetable_index < population->n_timetables;
       timetable_index++) {

    // Now calling each of the constraint functions for every time table index,
    // then modifying either the soft_fitness or hard_fitness arrays at the
    // appropriate index and also updating the averages for each of the
    // constraints

    // We make the time table from the population
    TimeTableTuple timetable[population->n_sessions];

    // Query every tuple from the population and assign it to the timetable
    for (uint i = 0; i < population->n_sessions; i++) {
      uint venue, timeslot;
      ttGetTuple(population, timetable_index, i, &timeslot, &venue);
      timetable[i].timeslot = timeslot;
      timetable[i].venue = venue;
    }

    // HARD CONSTRAINTS

    average_hard_party_conflict +=
        hard_party_conflict(timetable, specifications, 0);
    hard_fitness[timetable_index] += hard_party_conflict(timetable, specifications, 0);

    average_hard_repeated_tuple +=
        hard_repeated_tuple(timetable, specifications, 0);
    hard_fitness[timetable_index] += hard_repeated_tuple(timetable, specifications, 0);

    average_hard_venue_capacity +=
        hard_venue_capacity(timetable, specifications, 0);
    hard_fitness[timetable_index] += hard_venue_capacity(timetable, specifications, 0);

    average_hard_venue_type += hard_venue_type(timetable, specifications, 0);
    hard_fitness[timetable_index] += hard_venue_type(timetable, specifications, 0);

    average_hard_max_hours += hard_max_hours(timetable, specifications, 0);
    hard_fitness[timetable_index] += hard_max_hours(timetable, specifications, 0);

    average_hard_multi_timeslot +=
        hard_multi_timeslot(timetable, specifications, 0);
    hard_fitness[timetable_index] += hard_multi_timeslot(timetable, specifications, 0);

    // SOFT CONSTRAINTS

    average_soft_travel_time +=
        soft_travel_time(timetable, specifications, 0);
    soft_fitness[timetable_index] += soft_travel_time(timetable, specifications, 0);
    
    average_soft_chunking += soft_chunking(timetable, specifications, 0);
    soft_fitness[timetable_index] += soft_chunking(timetable, specifications, 0);

    average_soft_room_utilization +=
        soft_room_utilization(timetable, specifications, 0);
    soft_fitness[timetable_index] += soft_room_utilization(timetable, specifications, 0);

    average_soft_extreme_time +=
        soft_extreme_time(timetable, specifications, 0);
    soft_fitness[timetable_index] += soft_extreme_time(timetable, specifications, 0);

    average_soft_room_capacity_utilization +=
        soft_room_capacity_utilization(timetable, specifications, 0);
    soft_fitness[timetable_index] += soft_room_capacity_utilization(timetable, specifications, 0);

    average_soft_sessions_well_distributed +=
        soft_sessions_well_distributed(timetable, specifications, 0);
    soft_fitness[timetable_index] += soft_sessions_well_distributed(timetable, specifications, 0);
    
  }

  // Averaging the fitnesses
  /*average_hard_party_conflict /= population->n_timetables;
  average_hard_repeated_tuple /= population->n_timetables;
  average_hard_venue_capacity /= population->n_timetables;
  average_hard_venue_type /= population->n_timetables;
  average_hard_max_hours /= population->n_timetables;
  average_hard_multi_timeslot /= population->n_timetables;
  
  average_soft_travel_time /= population->n_timetables;
  average_soft_chunking /= population->n_timetables;
  average_soft_room_utilization /= population->n_timetables;
  average_soft_extreme_time /= population->n_timetables;
  average_soft_room_capacity_utilization /= population->n_timetables;
  average_soft_sessions_well_distributed /= population->n_timetables;*/

  // Printing the averages with identation and an iteration number of something

  printf("Average Hard Party Conflict: %f\n", average_hard_party_conflict);
  printf("Average Hard Repeated Tuple: %f\n", average_hard_repeated_tuple);
  printf("Average Hard Venue Capacity: %f\n", average_hard_venue_capacity);
  printf("Average Hard Venue Type: %f\n", average_hard_venue_type);
  printf("Average Hard Max Hours: %f\n", average_hard_max_hours);
  printf("Average Hard Multi Timeslot: %f\n", average_hard_multi_timeslot);

  printf("Average Soft Travel Time: %f\n", average_soft_travel_time);
  printf("Average Soft Chunking: %f\n", average_soft_chunking);
  printf("Average Soft Room Utilization: %f\n", average_soft_room_utilization);
  printf("Average Soft Extreme Time: %f\n", average_soft_extreme_time);
  printf("Average Soft Room Capacity Utilization: %f\n", average_soft_room_capacity_utilization);
  printf("Average Soft Sessions Well Distributed: %f\n", average_soft_sessions_well_distributed);

}