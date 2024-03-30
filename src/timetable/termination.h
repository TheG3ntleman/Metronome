#ifndef TERMINATION_H
#define TERMINATION_H

#include "../common/utils.h"
#include "../common/config.h"

char terminationCondition(uint generation_number, uint population_size, GeneticSpecifications *gaSpecs);

//char terminationCondition(uint generation_number, uint population_size, numeric *delta_soft_fitness, numeric *delta_hard_fitness, GeneticSpecifications *gaSpecs);

#endif
