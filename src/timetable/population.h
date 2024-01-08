#ifndef POPULATION_H
#define POPULATION_H

#include <stdlib.h>

#include "utils.h"

typedef struct {

	uint size;
	uint tt_size;

	uint n_timeslots;
	uint n_venues;

	//uint *tuples;
	//unnecessary waste of memory
	//just use a bijective map (barely
	// any perf hit)
	
	uint *tt_indices;

} Population;

// Memory allocation and deallocation
Population *makePopulation(uint size, uint tt_size, uint n_timeslots, uint n_venues);
void deletePopulation(Population *population);

// Access particular individuals in the population

// Utility Functions
numeric calculateTimeTableSimilarity(Population *population, uint i1, uint i2);
void getTimeTableTuple(Population *population, uint tt_id, uint session_id, uint *venue_id, uint *timeslot_id);

#endif
