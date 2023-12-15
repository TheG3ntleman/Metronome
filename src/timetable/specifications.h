#ifndef SPECIFICATIONS_H
#define SPECIFICATIONS_H

#include <stdlib.h>

typedef struct {
	uint size;
	uint *party_id;
	uint *strength;
} PartyTable;

typedef struct {
	uint size;
	uint *venue_id;
	uint *venue_type;
	uint *capacity;
	uint *number_available;
} VenueTable;

typedef struct {
	uint size;
	uint *session_id;
	uint *venue_type;
	uint *duration;
} SessionTable;

typedef struct {
	uint size;
	uint *party_id;
	uint *session_id;
	uint *priority;
} AssignmentTable;

typedef struct {
	PartyTable *parties;
	VenueTable *venues;
	SessionTable *sessions;
	AssignmentTable *assignments;
} TimeTableSpecifications;

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
);

void deleteTimeTableSpecifications(TimeTableSpecifications *specs);
void printTimeTableSpecificationSummary(TimeTableSpecifications *specs);

#endif
