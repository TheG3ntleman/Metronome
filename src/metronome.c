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

	TimeTableSpecifications *specs = makeTimeTableSpecifications(
		p_size,
		p_party_id,
		p_strength,

		// For venue table
		v_size,
		v_venue_id,
		v_venue_type,
		v_capacity,
		v_number_available,

		// For session table
		s_size,
		s_session_id,
		s_venue_type,
		s_duration,

		// For assignments table
		a_size,
		a_party_id,
		a_session_id,
		a_priority
	);

	printTimeTableSpecificationSummary(specs);

	// Running GA/Evolution on the timetables
	GeneticSpecifications gaSpecs = {
		.population_size = 100,
		.selection_size = 10,
		.minimum_timetables_per_cluster = 5,
		.similarity_threshold = 40,
		.mutation_rate = 0.05,
		.maximum_generations = 100,
	};

	Population *population = evolveTimeTables(specs, &gaSpecs);
	
	// branch and bound or IP or return IP problem, cplex

	deleteTimeTableSpecifications(specs);

}
