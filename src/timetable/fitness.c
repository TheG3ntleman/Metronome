#include "fitness.h"
#include "population.h"

// Capacity constraint related fitness functions
#ifdef HARD_CAPACITY_CONSTRAINT
static numeric computeHardConstraint_CapacityConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {
	
    	// creating an array of array which conatains all the session and parties associated with them
    	uint session_party[population->n_sessions][15];
    	for (uint i = 0; i < population->n_sessions; i++) {
        	for (uint j = 0; j < 15; j++) {
            		session_party[i][j] = -1;
        	}
    	}
	
     	for (uint i = 0; i < population->n_sessions; i++) {
		session_party[i][0] = i;
		uint n = 1;
		for (uint j = 0; j < specifications->assignments->size; j++) {
	    		if (i == specifications->assignments->session_id[j]) {
				session_party[i][n] = specifications->assignments->party_id[j];
				n++;
	    		}
		}
    	}       

    	// converting from partyid to party strenght in session_party array
    	for (uint i = 1; i < 15; i++) {
        	for (uint j = 0; j < population->n_sessions; j++) {
            		for (uint k = 0; k < specifications->parties->size; k++){
                		if (specifications->parties->party_id[k] == session_party[j][i]) {
                    			session_party[j][i] = specifications->parties->strenght[k];
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

    	for (uint i = 0; i < population->n_sessions; i++) {
        	session_strenght[i][0] = session_party[i][0];
        	uint n = 0;
        	for (uint j = 1; j < 15; j++) {
            		if (session_party[i][j] != -1) {
                		n += session_party[i][j];
            		}
        	}
        	session_strenght[i][1] = n;
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
                		if (session_strenght[j][1] > specifications->venues->capacity[venue_id]) {
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
		uint venue_type = 0;
		uint venue_type_assigned = 0;
		for (uint j = 0; j < specifications->sessions->size; j++) {
			if (specifications->sessions->session_id[j] == i) {
				venue_type = specifications->sessions->venue_type[j];
				break;
			}
		}
		// getting the venue type for the venue assigned by the solver
		for (uint j = 0; j < specifications->venues->size; j++) {
			if (specifications->venues->venue_id[j] == venue) {
				venue_type_assigned = specifications->venues->venue_type[j];
				break;
			}
		}

		// check if they are matching
		if (venue_type != venue_type_assigned) {
			violations++;
		}
	} 

	return violations;
}
#endif

#ifdef HARD_MAX_SESSIONS 
static numeric computeHardConstraint_MaxSessionsConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {

    	// calculating number of teacher id's
    	uint teacher_number = 0;
    	for (uint i = 0; i < specifications->parties->size; i++) {
        	if (specifications->parties->strength[i] == 1) {
            		teacher_number++;
        	}
    	}

    	// creating an array that stores teacher party id and number of sessions per day
    	uint teacher_hours[teacher_number][6];
    	uint m = 0;
    	for (uint i = 0; i < specifications->parties->size; i++) {
        	if (specifications->parties->strength[i] == 1) {
            		teacher_hours[m][0] = specifications->parties->party_id[i];
            		for (uint j = 1; j < 6; j++) {
                		teacher_hours[m][j] = 0;
            		}
            		m++;
        	}
    	}

    	// creating an array of array which contains all the session and parties associated with them
    	uint session_party[population->n_sessions][15];
    	for (uint i = 0; i < population->n_sessions; i++) {
        	for (uint j = 0; j < 15; j++) {
            		session_party[i][j] = -1;
        	}
    	}

    	for (uint i = 0; i < population->n_sessions; i++) {
        	session_party[i][0] = i; 
        	uint n = 1; 
        	for (uint j = 0; j < specifications->assignments->size; j++) {
            		if (i == specifications->assignments->session_id[j]) {
                		session_party[i][n] = specifications->assignments->party_id[j]; 
                		n++;
            		}
        	}
    	}

    	// calculating how many session there are in each day for each teacher id
    	for (uint i = 0; i < teacher_number; i++) {
        	for (uint j = 0; j < population->n_sessions; j++) {
            		for (uint k = 1; k < 15; k++) {
                		if (teacher_hours[i][0] == session_party[j][k]) {
                    			uint timeslot, venue;
                    			getTimeTableTuple(population, timetable_index, session_party[j][0], &venue, &timeslot);
                    			uint day = timeslot / 7;
                    			teacher_hours[i][day]++;
                		}
            		}
        	}
    	}

    	// calculating violations by comparing the number of sessions held each day for each teacher with max number of sessions
    	numeric violations = 0;

    	for (uint i = 0; i < teacher_number; i++) {
        	for (uint j = 1; j < 6; j++) {
            		if (teacher_hours[i][j] > MAX_HOURS_PER_DAY) {
                		violations++;
            		}
        	}
    	}

    	return violations;
}
#endif


#ifdef HARD_PARTY_DUPLICATE
static numeric computeHardConstraint_PartyDuplicateConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {
	
    	// creating an array of array which conatains all the session and parties associated with them
    	uint session_party[population->n_sessions][15];
    	for (uint i = 0; i < population->n_sessions; i++) {
        	for (uint j = 0; j < 15; j++) {
            		session_party[i][j] = -1;
        	}
    	}

    	for (uint i = 0; i < population->n_sessions; i++) {
        	session_party[i][0] = i;
        	uint n = 1;
        	for (uint j = 0; j < specifications->assignments->size; j++) {
            		if (i == specifications->assignments->session_id[j]) {
                		session_party[i][n] = specifications->assignments->party_id[j];
                		n++;
            		}
        	}
    	} 	 

    	numeric violations = 0;

    	// Iterate through parties
	for (uint i = 0; i < population->n_sessions; i++) { 
		uint timeslot1, venue1, timeslot2, venue2;
		getTimeTableTuple(population, timetable_index, i, &venue1, &timeslot1);
		for (uint j = i + 1; j < population->n_sessions; j++) {
			getTimeTableTuple(population, timetable_index, j, &venue2, &timeslot2);
			if (timeslot1 == timeslot2) {
				for (uint k = 1; k < 15; k++) {
					if (session_party[i][k] == -1) {
						break;
					}
					for (uint w = 1; w < 15; w++) {
						if (session_party[j][w] == -1) {
							break;
						}
						if (session_party[i][k] == session_party[j][w]) {
							violations++;
						}
					}
				}
			}
		}
	}

    	return violations;
}
#endif

#ifdef HARD_SUFFECIENT_TIMESLOT
static numeric computeHardConstraint_sufficient_timeslotConstraint(Population *population, TimeTableSpecifications *specifications, uint timetable_index) {
    	numeric violations = 0;

    	for (uint i = 0; i < population->n_sessions; i++) {
        	uint timeslot1, venue1, timeslot2, venue2;
		getTimeTableTuple(population, timetable_index, i, &venue1, &timeslot1);
        	for (uint j = 0; j < specifications->sessions->size; j++) {
            		if (specifications->sessions->session_id[j] == i) {
                		if (specifications->sessions->duration[j] > 1) {
                    			uint duration = specifications->sessions->duration[j];
                    			for (uint k = i + 1; k < i + duration; k++) {
                        			getTimeTableTuple(population, timetable_index, k, &venue2, &timeslot2);
                        			if (timeslot1 + k - i != timeslot2) {
                            				violations++;
							if (venue1 != venue2) {
								violations++;
							}
                        			}
                        			else {
                            				if (venue1 != venue2) {
                                				violations++;
                            				}
                        			}
                    			}
                    			i = i + duration;
                		}
            		}
        	}
    	}

    	return violations;
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
#ifdef HARD_SUFFECIENT_TIMESLOT
	fitness += computeHardConstraint_sufficient_timeslotConstraint(population, specifications, timetable_index);
#endif
	return -fitness;

}

#ifdef SOFT_STUDENT_TRAVELTIME
static numeric computeSoftConstraint_studenttraveltime(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*Consecutive sessions may need to be scheduled in close proximity to each other to minimize student travel time between them.*/
	
	return 0;
}
#endif
#ifdef SOFT_MAXIMIZE_CHUNKING
static numeric computeSoftConstraint_maximize_chunking(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*maximize session chunking.(have back to back sessions for students)*/
	
	return 0;
}
#endif
#ifdef SOFT_ROOM_UTILIZATION
static numeric computeSoftConstraint_room_utilization(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*Maximize Room Utilization Constraint: Encourage the efficient use of rooms by maximizing their overall utilization.*/
	
	return 0;
}
#endif
#ifdef SOFT_AVOID_EARLYLATE_TIME
static numeric computeSoftConstraint_avoid_early_late_session(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*Avoid Early/Late Times Constraint: Express preferences to avoid scheduling classes too early or too late in the day.*/
	
	return 0;
}
#endif
#ifdef SOFT_ROOM_CAPACITY_UTILIZATION
static numeric computeSoftConstraint_room_capacity_utilization(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*avoid scheduling session with less strenght to venues which have high capacity ex: dont schedule ecm only class at auditorium*/
	
	return 0;
}
#endif
#ifdef SOFT_COMMON_TIMSLOT_EMPTY
static numeric computeSoftConstraint_common_timeslot_empty(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*encourage common time slots being left empty for a particular batch, 
 	so if there are some sessions that were canceled during the week, they can be held at that time slot.*/
	
	return 0;
}
#endif
#ifdef SOFT_MINIMIZE_BACKTOBACK_TEACHERCLASSES
static numeric computeSoftConstraint_backtoback_teacher_class(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*Minimization of Back-to-Back Classes for teachers.*/
	
	return 0;
}
#endif
#ifdef SOFT_MINIMIZE_SAMECOURSE_SESSION
static numeric computeSoftConstraint_samecouse_session(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*discourage scheduling sessions of the same course more than once per day.*/
	
	return 0;
}
#endif
#ifdef SOFT_LAB_AFTER_LECTURE
static numeric computeSoftConstraint_lab_after_lecture(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*have lab sessions after the lecture session for a particular course*/
	
	return 0;
}
#endif
#ifdef SOFT_SESSIONS_EVENLY_THROUGHOUT_WEEK
static numeric computeSoftConstraint_even_distrubution(Population *population, TimeTableSpecifications *specifications, uint timetable_index){
	/*have an even distribution of events throughout the week (avoid having too many sessions at one day)*/
	
	return 0;
}
#endif

void computeFitnesses(Population *population, TimeTableSpecifications *specfications, numeric *soft_fitness, numeric *hard_fitness) {

        //Iterate through all members of the population
	
	for (uint i = 0; i < population->size; i++) {
		soft_fitness[i] = 0;
		hard_fitness[i] = computeHardFitnesses(population, specfications, i);
	}


}
