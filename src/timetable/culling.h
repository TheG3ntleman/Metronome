#ifndef CULLING_H
#define CULLING_H

#include "utils.h"
#include "population.h"

void cullPopulation(Population *population, numeric *soft_fitness, numeric *hard_fitness, uint selection_size, uint *selection_arr);

#endif
