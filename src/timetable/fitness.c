#include "fitness.h"

static uint computeHardConstraint_PartyNonCollision(Population *population, TimeTableSpecifications, uint timetable_index){
  	uint n = 0;
	for(uint i = 0; i < population->size; i++){
		//getTimeTableTuple();
	}
}


static numeric computeSoftFitness(Population *population, TimeTableSpecifications *specifications,  uint timetable_index) {
}

static numeric computeHardFitness(Population *population, TimeTableSpecifications *specfications, uint timetable_index) {
}

void computeFitnesses(Population *population, TimeTableSpecifications *specfications, numeric *soft_fitness, numeric *hard_fitness) {

	// Iterate through all members of the population
	for (uint i = 0; i < population->size; i++) {
		//soft_fitness[i] = computeSoftFitness(population, i);
		//hard_fitness[i] = computeHardFitness(population, i);
	}
}
