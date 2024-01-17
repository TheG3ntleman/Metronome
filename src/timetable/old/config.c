#include "config.h"

// GASpecifications related functions
GASpecfications* makeGASpecifications() {

	GASpecfications *specs = (GASpecfications*)malloc(sizeof(GASpecfications));

	specs->population_size = 20;
	specs->selection_size = 10;
	specs->min_timetable_per_cluster = 3;

	specs->epsilon_soft = 1.0f;
	specs->epsilon_hard = 1.0f;

	return specs;
}

void deleteGASpecfications(GASpecfications *specs) {
	free(specs);
}


