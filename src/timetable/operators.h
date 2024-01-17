#ifndef OPERATORS_H
#define OPERATORS_H

#include "utils.h"
#include "config.h"
#include "specifications.h"
#include "population.h"

// Operators are functions that act on entire populations and return
// resultant populations. The resultants are not returned as they are rather populated in existing structures.

// Function to initialize population randomly
void operatorInitialize(Population *population, TimeTableSpecifications *ttSpecs, GeneticSpecifications *gaSpecs);

// Function for crossover operator
void operatorCrossover(Population *new_population, Population *current_population, uint *indices_of_selected_organisms, TimeTableSpecifications *ttSpecs, GeneticSpecifications *gaSpecs); 

// Function for mutation operator
void operatorMutation(Population *population, TimeTableSpecifications *ttSpecs, GeneticSpecifications *gaSpecs);

#endif
