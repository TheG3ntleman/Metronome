#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <stdio.h>

#include "utils.h"
#include "specifications.h"

#include "population.h"
#include "operators.h"
#include "fitness.h"
#include "culling.h"

typedef struct {
	uint population_size;
	uint selection_size;
} GASpecfications;

// GASpecifications related functions
GASpecfications* makeGASpecifications();
void deleteGASpecfications(GASpecfications *specs);

// Main Evolution Function
Population* evolveTimeTables(TimeTableSpecifications *specs, GASpecfications *ga_specs);

#endif
