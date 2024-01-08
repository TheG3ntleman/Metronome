#ifndef FITNESS_H
#define FITNESS_H

#include "utils.h"
#include "population.h"
#include "specifications.h"

void computeFitnesses(Population *population, TimeTableSpecifications * specifications, numeric *soft_fitness, numeric *hard_fitness);

#endif
