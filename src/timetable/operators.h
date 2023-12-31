#ifndef OPERATORS_H
#define OPERATORS_H

#include <stdlib.h>

#include "population.h"

// Could benefit from better naming scheme

void initializePopulation(Population *population);
Population *crossOver(Population *newPopulation, Population *population, uint *selected_organisms);
void mutation(Population *population);

#endif
