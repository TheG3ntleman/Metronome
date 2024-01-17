#include "population.h"

// Constructors and destructors.

Population *makePopulation(uint n_timetables, uint n_sessions, uint n_timeslots, uint n_venues) {
	// Allocating memory for parent struct
	Population *population = (Population*)malloc(sizeof(Population));

	// Allocating memory for sub-array (data_indices)
	population->data_indices = (uint*)malloc(sizeof(uint) * n_timetables * n_sessions * 2);

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
numeric ttCalculateSimilarity(Population *population, uint timetable_index_1, uint timetable_index_2) {

	// Counts the number of common tuples for now.
	uint similarity = 0;
	uint offset_1 = timetable_index_1 * 2 * population->n_sessions;
	uint offset_2 = timetable_index_2 * 2 * population->n_sessions;

	for (uint i = 0; i < population->n_sessions; i++) {
		if (population->data_indices[offset_1 + 2 * i] == population->data_indices[offset_2 + 2 * i] && population->data_indices[offset_1 + 2 * i + 1] == population->data_indices[offset_2 + 2 * i + 1]) {
			similarity++;
		}
	}

	return similarity;

}

// Getters and Setters
void ttGetTuple(Population *population, uint timetable_index, uint session_id, uint *timeslot_id, uint *venue_id) {

	if (session_id >= population->n_sessions) {
		printf("Invalid session id, exceeds total number of sessions: %u / %u\n", session_id, population->n_sessions);
		exit(ERR_SESSION_ID_OVERFLOW);
	}

	uint offset = timetable_index * 2 * population->n_sessions + 2 * session_id;

	*timeslot_id = population->data_indices[offset];
	*venue_id = population->data_indices[offset + 1];

}

void ttSetTuple(Population *population, uint timetable_index, uint session_id, uint timeslot_id, uint venue_id) {

	if (session_id >= population->n_sessions) {
		printf("Invalid session id, exceeds total number of sessions: %u / %u\n", session_id, population->n_sessions);
		exit(ERR_SESSION_ID_OVERFLOW);
	}

	uint offset = timetable_index * 2 * population->n_sessions + 2 * session_id;

	population->data_indices[offset] = timeslot_id;
	population->data_indices[offset + 1] = venue_id;

}

void ttSetVenue(Population *population, uint timetable_index, uint session_id, uint venue_id) {

	if (session_id >= population->n_sessions) {
		printf("Invalid session id, exceeds total number of sessions: %u / %u\n", session_id, population->n_sessions);
		exit(ERR_SESSION_ID_OVERFLOW);
	}

	uint offset = timetable_index * 2 * population->n_sessions + 2 * session_id;

	population->data_indices[offset + 1] = venue_id;
}

void ttSetTimeslot(Population *population,  uint timetable_index, uint session_id, uint timeslot_id) {

	if (session_id >= population->n_sessions) {
		printf("Invalid session id, exceeds total number of sessions: %u / %u\n", session_id, population->n_sessions);
		exit(ERR_SESSION_ID_OVERFLOW);
	}

	uint offset = timetable_index * 2 * population->n_sessions + 2 * session_id;

	population->data_indices[offset] = timeslot_id;

}

void ttSet(Population *population, uint timetable_index, uint *venue_ids, uint *timeslot_ids) {


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
		printf("\t%u - %u", timeslot_id, venue_id);
	}
}
