#include "specifications.h"
#include <stdio.h>

static uint *mallocColumn(uint size) {
	uint *arr = (uint*)malloc(sizeof(uint) * size);
	return arr;
}

static void copy(uint *from, uint *to, uint size) {
	for (uint i = 0; i < size; i ++) {
		to[i] = from[i];
	}
}

static uint* set(uint *src, uint size) {
	uint *res = mallocColumn(size);
	copy(src, res, size);
	return res;
}

TimeTableSpecifications *makeTimeTableSpecifications(
	// For party table
	size_t p_size,
	uint *p_party_id,
	uint *p_strength,

	// For venue table
	size_t v_size,
	uint *v_venue_id,
	uint *v_venue_type,
	uint *v_capacity,
	uint *v_number_available,

	// For session table
	size_t s_size,
	uint *s_session_id,
	uint *s_venue_type,
	uint *s_duration,

	// For assignments table
	size_t a_size,
	uint *a_party_id,
	uint *a_session_id,
	uint *a_priority
) {

	TimeTableSpecifications *specs = (TimeTableSpecifications*)malloc(sizeof(TimeTableSpecifications));

	// Making the party table
	PartyTable *parties = (PartyTable*)malloc(sizeof(PartyTable));

	parties->size = p_size;
	parties->party_id = set(p_party_id, p_size);
	parties->strength = set(p_strength, p_size);

	// Making the venue table
	VenueTable *venues = (VenueTable*)malloc(sizeof(VenueTable));

	venues->size = v_size;
	venues->venue_id = set(v_venue_id, v_size);
	venues->venue_type = set(v_venue_type, v_size);
	venues->capacity = set(v_capacity, v_size);
	venues->number_available = set(v_number_available, v_size);
	
	// Making the session table
	SessionTable *sessions = (SessionTable*)malloc(sizeof(SessionTable));

	sessions->size = s_size;
	sessions->session_id = set(s_session_id, s_size);
	sessions->venue_type = set(s_venue_type, s_size);
	sessions->duration = set(s_duration, s_size);

	// Making the assignment table
	AssignmentTable *assignments = (AssignmentTable*)malloc(sizeof(AssignmentTable));

	assignments->size = a_size;
	assignments->party_id = set(a_party_id, a_size);
	assignments->session_id = set(a_session_id, a_size);
	assignments->priority = set(a_priority, a_size);

	// Connecting specification with all tables
	specs->parties = parties;
	specs->venues = venues;
	specs->sessions = sessions;
	specs->assignments = assignments;

	return specs;

}

void deleteTimeTableSpecifications(TimeTableSpecifications *specs) {

	free(specs->parties->party_id);
	free(specs->parties->strength);
	free(specs->venues->venue_id);
	free(specs->venues->venue_type);
	free(specs->venues->capacity);
	free(specs->venues->number_available);
	free(specs->sessions->session_id);
	free(specs->sessions->venue_type);
	free(specs->sessions->duration);
	free(specs->assignments->party_id);
	free(specs->assignments->session_id);
	free(specs->assignments->priority);
	free(specs->parties);
	free(specs->venues);
	free(specs->sessions);
	free(specs->assignments);
	free(specs);

}

void printTimeTableSpecificationSummary(TimeTableSpecifications *specs) {
	printf("\nTime Table Specifications:\n");
	printf("\tThere are %u parties.\n", specs->parties->size);
	printf("\tThere are %u venues.\n", specs->venues->size);
	printf("\tThere are %u sessions/periods to be scheduled.\n", specs->sessions->size);
	printf("\tThere are %u assignments.\n", specs->assignments->size);
}

