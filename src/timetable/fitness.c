#include "fitness.h"

static uint computerHardConstraint_PartyNonCollision(Population *population, TimeTableSpecfications *specifications, uint timetable_index) {

	// Find timeslots
	for (uint i = 0; i < population->size_timetable; i++) {
		for (uint j = 0; j < population->size_timetable; j++) {
			
		}
	}

}

static numeric computeSoftFitness(Population *population, TimeTableSpecfications,  uint timetable_index) {
}

static numeric computeHardFitness(Population *population, TimeTableSpecifications *specfications, uint timetable_index) {
}

void computeFitnesses(Population *population, TimeTableSpecifications *specfications, numeric *soft_fitness, numeric *hard_fitness) {

	// Iterate through all members of the population
	for (uint i = 0; i < population->size; i++) {
		soft_fitness[i] = computeSoftFitness(population, i);
		hard_fitness[i] = computeHardFitness(population, i);
	}

}
