#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "config.h"
#include "utils.h"
#include "population.h"
#include "specifications.h"
#include "operators.h"
#include "termination.h"
#include "fitness.h"
#include "culling.h"

Population *evolveTimeTables(TimeTableSpecifications *ttSpecs, GeneticSpecifications *gaSpecs);

#endif
