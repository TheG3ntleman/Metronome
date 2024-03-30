#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "../common/config.h"
#include "../common/utils.h"
#include "population.h"
#include "../common/specifications.h"
#include "operators.h"
#include "termination.h"
#include "fitness.h"
#include "culling.h"

Population *evolveTimeTables(TimeTableSpecifications *ttSpecs, GeneticSpecifications *gaSpecs);

#endif
