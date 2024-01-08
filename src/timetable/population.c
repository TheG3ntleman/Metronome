#include "population.h"

// Memory allocation and deallocation
Population *makePopulation(uint size, uint tt_size, uint n_timeslots, uint n_venues) {

	
	Population *population = (Population*)malloc(sizeof(Population));

	population->size = size;
	population->tt_size = tt_size;
	population->n_timeslots = n_timeslots;
	population->n_venues = n_venues;

	population->tt_indices = (uint*)malloc(sizeof(tt_size * size));

	return population;

}


void deletePopulation(Population *population) {
	free(population->tt_indices);
	free(population);
}

// Access particular individuals in the population

// Utility Functions
numeric calculateTimeTableSimilarity(Population *population, uint i1, uint i2) {

	// Essentially records number of tuple matches
	numeric similarity = 0;
	
	for (uint i = 0; i < population->tt_size; i++) {
		if (population->tt_indices[i1 * population->tt_size + i] == population->tt_indices[i2 * population->tt_size + i])
			similarity++;
	}

	return similarity;

}

void getTimeTableTuple(Population *population, uint tt_id, uint session_id, uint *venue_id, uint *timeslot_id) {

	uint tuple_index = population->tt_indices[population->tt_size * tt_id + session_id];

	*timeslot_id = tuple_index / population->n_venues;
	*venue_id = tuple_index - (*timeslot_id) * population->n_venues; 

}



