#ifndef POPULATION_H
#define POPULATION_H

#include <stdio.h>
#include <stdlib.h>

#include "../common/config.h"
#include "../common/utils.h"

/*
 * These functions do not enforce the usage of
 * true/real timeslot and venue ids. It is expected
 * that the caller is using appropriate conventions
 * as per the specifications supplied.
 *
 * Also, timeslots and venues are stored in the same array 
 * in an alternating fashion. timeslot_id values are 
 * followed by their venue_id values.
 *
 * In general all indexing starts from 0.
 * */

typedef struct {

	// Dimension of population
	uint n_timetables;
	
	// Dimensions of organisms
	uint n_sessions;
	uint n_timeslots;
	uint n_venues;

	// Actual data is stored here.
	uint *data_indices;

} Population;

// Constructors and destructors.

Population *makePopulation(uint n_timetables, uint n_sessions, uint n_timeslots, uint n_venues);
void deletePopulation(Population *population);

// Metric/Similarity Function
numeric ttCalculateSimilarity(Population *population, uint timetable_index_1, uint timetable_index_2);

// Getters and Setters
void ttGetTuple(Population *population, uint timetable_index, uint session_id, uint *timeslot_id, uint *venue_id);
void ttSetTuple(Population *population, uint timetable_index, uint session_id, uint timeslot_id, uint venue_id);
void ttSetVenue(Population *population, uint timetable_index, uint session_id, uint venue_id);
void ttSetTimeslot(Population *population,  uint timetable_index, uint session_id, uint timeslot_id);
void ttSet(Population *population, uint timetable_index, uint *venue_ids, uint *timeslot_ids);

// IO
void ttPrint(Population *population, uint timetable_index);

#endif
