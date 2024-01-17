#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <stdio.h>

#include "utils.h"
#include "specifications.h"

#include "population.h"
#include "operators.h"
#include "fitness.h"
#include "culling.h"
// Main Evolution Function
Population* evolveTimeTables(TimeTableSpecifications *specs, GASpecfications *ga_specs);

#endif
