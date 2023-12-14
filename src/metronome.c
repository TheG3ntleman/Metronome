#include "timetable/specifications.h"
#include "metronome.h"
#include <stdio.h>

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
) {

	printf("I am here!\n");
	printf("I am counting %u parties!\n", p_size);
	for (uint i = 0; i < p_size; i++) {
		printf("%u\n", p_strength[i]);
	}

}
