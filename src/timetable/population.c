#include "population.h"
#include "../common/specifications.h"
#include "../common/constraint.h"
#include <math.h>
#include "../common/specifications.h"
#include "../common/constraint.h"
#include "utils.h"
#define coef 0.2
// Constructors and destructors.

Population *makePopulation(uint n_timetables, uint n_sessions, uint n_timeslots,
                           uint n_venues) {
  // Allocating memory for parent struct
  Population *population = (Population *)malloc(sizeof(Population));

  // Allocating memory for sub-array (data_indices)
  population->data_indices =
      (uint *)malloc(sizeof(uint) * n_timetables * n_sessions * 2);

  // Copying info into new struct
  population->n_timetables = n_timetables;
  population->n_sessions = n_sessions;
  population->n_timeslots = n_timeslots;
  population->n_venues = n_venues;

  // Initializing array
  for (uint i = 0; i < n_timetables * n_sessions * 2; i++)
    population->data_indices[i] = 0;

  return population;
}

void deletePopulation(Population *population) {
  free(population->data_indices);
  free(population);
}

// Metric/Similarity Function
numeric ttCalculateSimilarity(Population *population, uint timetable_index_1,
                              uint timetable_index_2) {

  // Counts the number of common tuples for now.
  uint similarity = 0;
  uint offset_1 = timetable_index_1 * 2 * population->n_sessions;
  uint offset_2 = timetable_index_2 * 2 * population->n_sessions;

  for (uint i = 0; i < population->n_sessions; i++) {
    if (population->data_indices[offset_1 + 2 * i] ==
            population->data_indices[offset_2 + 2 * i] &&
        population->data_indices[offset_1 + 2 * i + 1] ==
            population->data_indices[offset_2 + 2 * i + 1]) {
      similarity++;
    }
  }

  return similarity;
}

// Getters and Setters
void ttGetTuple(Population *population, uint timetable_index, uint session_id,
                uint *timeslot_id, uint *venue_id) {

  if (session_id >= population->n_sessions) {
    printf("Invalid session id, exceeds total number of sessions: %u / %u\n",
           session_id, population->n_sessions);
    exit(ERR_SESSION_ID_OVERFLOW);
  }

  uint offset = timetable_index * 2 * population->n_sessions + 2 * session_id;

  *timeslot_id = population->data_indices[offset];
  *venue_id = population->data_indices[offset + 1];
}

void ttSetTuple(Population *population, uint timetable_index, uint session_id,
                uint timeslot_id, uint venue_id) {

  if (session_id >= population->n_sessions) {
    printf("Invalid session id, exceeds total number of sessions: %u / %u\n",
           session_id, population->n_sessions);
    exit(ERR_SESSION_ID_OVERFLOW);
  }

  uint offset = timetable_index * 2 * population->n_sessions + 2 * session_id;

  population->data_indices[offset] = timeslot_id;
  population->data_indices[offset + 1] = venue_id;
}

void ttSetVenue(Population *population, uint timetable_index, uint session_id,
                uint venue_id) {

  if (session_id >= population->n_sessions) {
    printf("Invalid session id, exceeds total number of sessions: %u / %u\n",
           session_id, population->n_sessions);
    exit(ERR_SESSION_ID_OVERFLOW);
  }

  uint offset = timetable_index * 2 * population->n_sessions + 2 * session_id;

  population->data_indices[offset + 1] = venue_id;
}

void ttSetTimeslot(Population *population, uint timetable_index,
                   uint session_id, uint timeslot_id) {

  if (session_id >= population->n_sessions) {
    printf("Invalid session id, exceeds total number of sessions: %u / %u\n",
           session_id, population->n_sessions);
    exit(ERR_SESSION_ID_OVERFLOW);
  }

  uint offset = timetable_index * 2 * population->n_sessions + 2 * session_id;

  population->data_indices[offset] = timeslot_id;
}

void ttSet(Population *population, uint timetable_index, uint *venue_ids,
           uint *timeslot_ids) {

  uint offset = timetable_index * 2 * population->n_sessions;

  for (uint i = 0; i < population->n_sessions; i++) {
    population->data_indices[offset + 2 * i] = timeslot_ids[i];
    population->data_indices[offset + 2 * i + 1] = venue_ids[i];
  }
}

// IO

void ttPrint(Population *population, uint timetable_index) {
  printf("TimeTable %u :\n", timetable_index);
  for (uint i = 0; i < population->n_sessions; i++) {
    uint timeslot_id, venue_id;
    ttGetTuple(population, timetable_index, i, &timeslot_id, &venue_id);
    printf("\t%u - %u\n", timeslot_id, venue_id);
  }
}


snumeric sigmoidWithCoefficient(snumeric x, snumeric coefficient){
  return 1 / (1 + pow(exp(1), -coefficient * x));
}


snumeric calculateHardFitness(TimeTableTuple *timetable, TimeTableSpecifications *specs, uint depth){
  snumeric accumulate_fitness = 0;
  
  uint hard_party_conflict_val = hard_party_conflict(timetable, specs, depth);
  uint hard_repeated_tuple_val = hard_repeated_tuple(timetable, specs, depth);
  uint hard_venue_capacity_val = hard_venue_capacity(timetable, specs, depth);
  uint hard_venue_type_val = hard_venue_type(timetable, specs, depth);
  uint hard_max_hours_val = hard_max_hours(timetable, specs, depth);
  uint hard_multi_timeslot_val = hard_multi_timeslot(timetable, specs, depth);



  accumulate_fitness += ((sigmoidWithCoefficient(hard_party_conflict_val, coef)) + 
                          sigmoidWithCoefficient(hard_repeated_tuple_val, coef) +
                          sigmoidWithCoefficient(hard_venue_capacity_val, coef) +
                          sigmoidWithCoefficient(hard_venue_type_val, coef) +
                          sigmoidWithCoefficient(hard_max_hours_val, coef) +
                          sigmoidWithCoefficient(hard_multi_timeslot_val, coef) 
                        );


  return accumulate_fitness;

}

snumeric calculateSoftFitness(TimeTableTuple *timetable, TimeTableSpecifications *specs, uint depth){
  snumeric accumulate_fitness = 0;
  
  uint soft_travel_time_val = soft_travel_time(timetable, specs, depth);
  uint soft_chunking_val = soft_chunking(timetable, specs, depth);
  uint soft_room_utilization_val = soft_room_utilization(timetable, specs, depth);
  uint soft_extreme_time_val = soft_extreme_time(timetable, specs, depth);
  uint soft_room_capacity_utilization_val = soft_room_capacity_utilization(timetable, specs, depth);
  uint soft_common_timeslot_empty_val = soft_common_timeslot_empty(timetable, specs, depth);
  uint soft_minimize_back_to_back_val = soft_minimize_back_to_back(timetable, specs, depth);
  uint soft_repeated_course_session_val = soft_repeated_course_session(timetable, specs, depth); 
  uint soft_sessions_well_distributed_val = soft_sessions_well_distributed(timetable, specs, depth);
  uint soft_lab_after_lecture_val = soft_lab_after_lecture(timetable, specs, depth);
  accumulate_fitness += ((sigmoidWithCoefficient(soft_travel_time_val, coef)) + 
                          sigmoidWithCoefficient(soft_chunking_val, coef) +
                          sigmoidWithCoefficient(soft_room_utilization_val, coef) +
                          sigmoidWithCoefficient(soft_extreme_time_val, coef) +
                          sigmoidWithCoefficient(soft_room_capacity_utilization_val, coef) +
                          sigmoidWithCoefficient(soft_common_timeslot_empty_val, coef) +
                          sigmoidWithCoefficient(soft_minimize_back_to_back_val, coef) +
                          sigmoidWithCoefficient(soft_repeated_course_session_val, coef) +
                          sigmoidWithCoefficient(soft_sessions_well_distributed_val, coef) +
                          sigmoidWithCoefficient(soft_lab_after_lecture_val, coef)
                        );

  return accumulate_fitness;

}


void get_Hard_Soft_Fitness(Population *population, TimeTableSpecifications *specs, uint depth, uint *hardFitnessArray, uint *softFitnessArray){

  for(uint i = 0; i < population->n_timetables;i++){
    TimeTableTuple *timetableArray = (TimeTableTuple *)malloc(sizeof(TimeTableTuple) * population->n_sessions);
    for(uint j = 0; j < population->n_sessions; j++){
      TimeTableTuple temp = {0,0};
      ttGetTuple(population, i, j, &temp.timeslot, &temp.venue);
      timetableArray[j] = temp;
    }
    hardFitnessArray[i] = calculateHardFitness(timetableArray, specs, depth);
    softFitnessArray[i] = calculateSoftFitness(timetableArray, specs, depth);

  } 






}


