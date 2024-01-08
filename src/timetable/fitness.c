#include "fitness.h"


static uint computeHardConstraint_PartyNonCollision(Population *population, TimeTableSpecifications *specs, uint timetable_index){
        uint n = 0;
        for(uint i = 0; i < population->tt_size; i++){
                uint *venue_id_i;
                uint *timeslot_id_i;
                getTimeTableTuple(population, timetable_index, i, venue_id_i, timeslot_id_i);
                for(uint j = i+1; j < population->tt_size; j++){
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

static uint computeHardConstraint_venuecapacity(Population *population, TimeTableSpecifications *specs, uint timetable_index){
        uint n = 0;
        uint session_id;
        for(uint i = 0; i < population->tt_size; i++){

                session_id = i;
                uint *venue_id;
                uint *timeslot_id;
		getTimeTableTuple(population, timetable_index, session_id, venue_id, timeslot_id);

                // getting venue capacity
                uint venue_capacity = 0;
                for (uint j = 0; j < specs->venues->size; j++) {
                        if (specs->venues->venue_id[j] == venue_id) {
                                venue_capacity = specs->venues->capacity[j];
                                break;
                        }
                }

                // Calculate the total strength for the session
                uint total_strength = 0;
                for (uint j = 0; j < specs->assignments->size; j++) {
                        if (specs->assignments->session_id[j] == session_id) {
                                uint party_id = specs->assignments->party_id[j];
                                for (uint k = 0; k < specs->parties->size; k++) {
                                        if (specs->parties->party_id[k] == party_id) {
                                                total_strength += specs->parties->strength[k];
                                                break;
                                        }
                                }
                        }
                }
                
                // increment n if strenght is higher than capacity of venue
                if (total_strength > capacity){
                        n++;
                }
        }
        return n;
}

static uint computerHardConstraint_SuccesiveMultiTimeSlotClasses(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {
	
} 


static numeric computeSoftFitness(Population *population, TimeTableSpecifications *specifications,  uint timetable_index) {
}

static numeric computeHardFitness(Population *population, TimeTableSpecifications *specfications, uint timetable_index) {
}

void computeFitnesses(Population *population, TimeTableSpecifications *specfications, numeric *soft_fitness, numeric *hard_fitness) {

        //Iterate through all members of the population
        for (uint i = 0; i < population->size; i++) {
                //soft_fitness[i] = computeSoftFitness(population, i);
                //hard_fitness[i] = computeHardFitness(population, i);
        }
}
