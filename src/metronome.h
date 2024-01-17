#ifndef METRONOME_H
#define METRONOME_H

#include "timetable/config.h"
#include "timetable/evolution.h"
#include <stdlib.h>

void solveTimeTablingProblem(
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

#endif
