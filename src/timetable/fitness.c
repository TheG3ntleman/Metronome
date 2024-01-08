#include "fitness.h"

static uint computeHardConstraint_PartyNonCollision(Population *population, TimeTableSpecifications, uint timetable_index){
  	uint n = 0;
		for(uint i = 0; i < population->tt_size; i++){
        		uint *venue_id_i;
        		uint *timeslot_id_i;
			getTimeTableTuple(population, timetable_index, i, venue_id_i, timeslot_id_i);
        		for(uint j = i+1; j < population->size; j++){
            			uint *venue_id_j;
            			uint *timeslot_id_j;
            			getTimeTableTuple(population, timetable_index, j, venue_id_j, timeslot_id_j);
            			if (timeslot_id_i == timeslot_id_j && venue_id_i == venue_id_j){
                			n++;
            			}
        		}
    		}
    	return n;
}


static numeric computeSoftFitness(Population *population, TimeTableSpecifications *specifications,  uint timetable_index) {
}

static numeric computeHardFitness(Population *population, TimeTableSpecifications *specfications, uint timetable_index) {
}

void computeFitnesses(Population *population, TimeTableSpecifications *specfications, numeric *soft_fitness, numeric *hard_fitness) {

	Iterate through all members of the population
	for (uint i = 0; i < population->size; i++) {
		soft_fitness[i] = computeSoftFitness(population, i);
		hard_fitness[i] = computeHardFitness(population, i);
	}
}
