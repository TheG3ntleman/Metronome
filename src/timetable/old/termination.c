#include "termination.h"

char terminationCondition(uint generation_number, GASpecfications *ga_specs, Population *population, numeric *delta_soft_fitness, numeric *delta_hard_fitness) {

	if (generation_number > 0) {
		numeric max_delta_hard = 0;
		numeric max_delta_soft = 0;
		// Finding max deltas in both categories
		
		for (uint i = 0; i < population->size; i++) {
			if (max_delta_hard < delta_hard_fitness[i]) {
				max_delta_hard = delta_hard_fitness[i];
			}

			if (max_delta_soft < delta_soft_fitness[i])
				max_delta_soft = delta_soft_fitness[i];
		}

		if (max_delta_hard < ga_specs->epsilon_hard && max_delta_soft < ga_specs->epsilon_soft)
			return 1;
		else
		 	return 0;

		
	} else 
		return 0;

}
