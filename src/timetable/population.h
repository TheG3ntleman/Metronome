#ifndef POPULATION_H
#define POPULATION_H

#include <stdlib.h>

typedef struct {
} Population;

// Memory allocation and deallocation
Population *makePopulation(uint number_of_organisms);
void deletePopulation(Population *population);

#endif
