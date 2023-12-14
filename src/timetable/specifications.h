#ifndef SPECIFICATIONS_H
#define SPECIFICATIONS_H

#include <stdlib.h>

typedef struct {
	uint *party_id;
	uint *strength;
} parties;

typedef struct {
	uint *venue_id;
	uint *venue_type;
	uint *capacity;
	uint *number_available;
} venues;

typedef struct {
	uint *session_id;
	uint *venue_type;
	uint *duration;
} sessions;

typedef struct {
	uint *party_id;
	uint *session_id;
	uint *priority;
} assignments;

#endif
