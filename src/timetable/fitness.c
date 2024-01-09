#include "fitness.h"
#include "population.h"

// Capacity constraint related fitness functions
#ifdef HARD_CAPACITY_CONSTRAINT
static numeric computeHardConstraint_CapacityConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {
	
    	// creating an array of array which conatains all the session and parties associated with them
    	uint session_party[population->n_sessions][10];
    	for (uint i = 0; i < population->n_sessions; ++i) {
        	for (uint j = 0; j < 10; ++j) {
            		session_party[i][j] = -1;
        	}
    	}

     	for (uint i = 0; i < population->n_sessions; i++) {
		session_party[i][0] = i
		uint n = 0;
		for (uint j = 0; j < specifications->assignments->size, j++) {
	    		if (i == specifications->assignments->session_id[j]) {
				session_party[i][n] = specifications->assignments->party_id[j]
				n++;
	    		}
		}
    	}       

    	// converting from partyid to party strenght in session_party array
    	for (uint i = 1; i < 10; i++) {
        	for (uint j = 0; j < population->n_sessions; j++) {
            		for (uint k = 0; k < specifications->parties->size; k++){
                		if (specifications->parties->party_id[k] == session_party[j][i]) {
                    			session_party[j][i] = specifications->parties->strenght[k]
                		}
            		}
        	}
    	}

    	// creating an array of array which contains all the sessions and the strenghts associated with them
    	uint session_strenght[population->n_sessions][2];
    	for (uint i = 0; i < population->n_sessions; ++i) {
        	for (uint j = 0; j < 2; ++j) {
            	session_strenght[i][j] = -1;
        	}
    	}

    	for (uint i = 0; i < population->sessions; i++) {
        	session_strenght[i][0] = session_party[i][0]
        	uint n = 0;
        	for (uint j = 1; j < 10; j++) {
            		if (session_party[i][j] != -1) {
                	n += session_party[i][j]
            		}
        	}
        	session_strenght[i][1] = n
    	}

    	numeric violations = 0;

    	//comapring strenght for each session with the capacity of the venue its scheduled at
    	for (uint i = 0; i < population->n_sessions; i++) {
		uint timeslot_id;
		uint venue_id;
		getTimeTableTuple(population, timetable_index, i, &venue_id, &timeslot_id);
        	for (uint j = 0; j < population->n_sessions; j++) {
            		if (session_strenght[j][0] == i) {
                		// Final incrementation
                		if (session_strength[j][1] > specifications->venues->capacity[venue_id]) {
                    			violations++;
                		}
            		}
        	}
    	}

    	return violations;

}
#endif

#ifdef HARD_VENUE_CONFLICT
static numeric computeHardConstraint_VenueConflictConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {
	
	numeric violations = 0;

	// Iterating through sessions
	for (uint i = 0; i < population->n_sessions; i++) {
		uint timeslot1, venue1, timeslot2, venue2;
		getTimeTableTuple(population, timetable_index, i, &venue1, &timeslot1);
		for (uint j = i + 1; j < population->n_sessions; j++) {
			getTimeTableTuple(population, timetable_index, j, &venue2, &timeslot2);
			if (timeslot1 == timeslot2 && venue1 == venue2) {
				violations++;	
			}
		}
	}

	return violations;

}
#endif

#ifdef HARD_VENUE_TYPE
static numeric computeHardConstraint_VenueTypeConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {
	
	numeric violations = 0;

	// Iterating through sessions
	for (uint i = 0; i < population->n_sessions; i++) {

		uint timeslot, venue;
		getTimeTableTuple(population, timetable_index, i, &venue, &timeslot);

		// Retrieve required venue type
		uint venue_type;
		for (uint j = 0; j < specifications->sessions->size; j++) {
			if (specifications->sessions->session_id[j] == i) {
				venue_type = specifications->sessions->venue_type[j];
				break;
			}
		}

		// check if they are matching
		if (venue_type != venue)
			violations++;


	} 

	return violations;
}
#endif

#ifdef HARD_MAX_SESSIONS 
static numeric computeHardConstraint_MaxSessionsConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {

	// Making array to record the number of sessions per day
	uint hours[specifications->parties->size][NUMBER_OF_DAYS];
	for (uint i = 0; i < specifications->parties->size; i++) {
		for (uint j = 0; j < NUMBER_OF_DAYS; j++) {
			hours[i][j] = 0;
		}
	}

	// Actually recording the hours now.
	// Iterating through sessions
	for (uint i = 0; i < population->n_sessions; i++) {
		uint timeslot, venue;
		uint day = timeslot / 7.1f;
		getTimeTableTuple(population, timetable_index, i, &venue, &timeslot);

		uint party_id;
		for (uint j = 0; j < specifications->assignments->size; j++) {
			if (specifications->assignments->session_id[j] == i) {
				party_id = specifications->assignments->party_id[j];
				hours[party_id][day]++;	
			}
		}
	}

	// Searching for violations
	numeric violations = 0;

	for (uint i = 0; i < specifications->parties->size; i++) {
		for (uint j = 0; j < NUMBER_OF_DAYS; j++) {
			if (hours[i][j] > MAX_HOURS_PER_DAY)
				violations++;
		}
	}

	return violations;

}
#endif


#ifdef HARD_PARTY_DUPLICATE
static numeric computeHardConstraint_PartyDuplicateConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {
	
	numeric violations = 0;

	// Iterate through parties
	/*for (uint i = 0; i < population->n_sessions; i++) { 
		uint timeslot1, venue1, timeslot2, venue2;
		getTimeTableTuple(population, timetable_index, i, &venue1, &timeslot1);
		for (uint j = i + 1; j < population->n_sessions; j++) {
			getTimeTableTuple(population, timetable_index, j, &venue2, &timeslot2);
			for (uint k = 0; k < specifications->assignments->size; k++) {
				for (uint w = k + 1; w < specifications->assignments->size; w++) {
					if (specifications->assignments->session_id[k] )
				}
			}
		}
	}

	return violations;*/

	return  0;
}
#endif

static numeric computeHardFitnesses(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {

	numeric fitness = 0;


#ifdef HARD_CAPACITY_CONSTRAINT
	fitness += computeHardConstraint_CapacityConstraint(population, specifications, timetable_index);
#endif

#ifdef HARD_VENUE_CONFLICT
	fitness += computeHardConstraint_VenueConflictConstraint(population, specifications, timetable_index);
#endif

#ifdef HARD_VENUE_TYPE
	fitness += computeHardConstraint_VenueTypeConstraint(population, specifications, timetable_index); 
#endif

#ifdef HARD_MAX_SESSIONS
	fitness += computeHardConstraint_MaxSessionsConstraint(population, specifications, timetable_index); 
#endif

#ifdef HARD_PARTY_DUPLICATE
	fitness += computeHardConstraint_PartyDuplicateConstraint(population, specifications, timetable_index); 
#endif

	return -fitness;

}

void computeFitnesses(Population *population, TimeTableSpecifications *specfications, numeric *soft_fitness, numeric *hard_fitness) {

        //Iterate through all members of the population
	
	for (uint i = 0; i < population->size; i++) {
		soft_fitness[i] = 0;
		hard_fitness[i] = computeHardFitnesses(population, specfications, i);
	}


}
