#ifndef FITNESS_H
#define FITNESS_H

#include "../common/utils.h"
#include "../common/config.h"
#include "population.h"
#include "../common/specifications.h"

void computeFitnesses(Population *population, TimeTableSpecifications * specifications, numeric *soft_fitness, numeric *hard_fitness);

#endif
