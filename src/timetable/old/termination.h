#ifndef TERMINATION_H
#define TERMINATION_H

#include "config.h"
#include "utils.h"
#include "population.h"

char terminationCondition(uint generation_number, GASpecfications *ga_specs, Population *population, numeric *delta_soft_fitness, numeric *delta_hard_fitness);

#endif
