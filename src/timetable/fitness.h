#ifndef FITNESS_H
#define FITNESS_H

#include "../common/utils.h"
#include "population.h"
#include "../common/specifications.h"
#include "../common/constraint.h"

void computeFitnesses(Population *population, TimeTableSpecifications * specifications, numeric *soft_fitness, numeric *hard_fitness);

#endif
