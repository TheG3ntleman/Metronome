#ifndef TERMINATION_H
#define TERMINATION_H

#include "utils.h"
#include "population.h"

char terminationCondition(uint generation_number, numeric *delta_soft_fitness, numeric *delta_hard_fitness);

#endif
