#ifndef TERMINATION_H
#define TERMINATION_H

#include "utils.h"
#include "config.h"

#ifndef POPULATION_H_INCLUDED
#define POPULATION_H_INCLUDED
#include "population.h"
#endif

char terminationCondition(uint generation_number, uint population_size, GeneticSpecifications *gaSpecs);

char terminationCondition_temp(uint generation_number, uint population_size, numeric *delta_soft_fitness, numeric *delta_hard_fitness, GeneticSpecifications *gaSpecs, Population *population);

#endif
