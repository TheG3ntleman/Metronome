#ifndef CULLING_H
#define CULLING_H

#include "../common/utils.h"
#include "../common/config.h"
#include "population.h"

typedef struct {

	//  Array of the indices of the timetables in this cluster.
	uint *timetable_indices;

	// Number of timetables in this cluster. 
	uint size;

	// Capacity of the timetables array?
	uint capacity;

} Cluster;

void cullPopulationViaClustering(Population *population, numeric *soft_fitness, numeric *hard_fitness, uint *indices_of_selected_timetables, GeneticSpecifications *gaSpecs);

void cullPopulationViaSelection(Population *population, numeric *soft_fitness, numeric *hard_fitness, uint *indices_of_selected_timetables, GeneticSpecifications *gaSpecs);

#endif
