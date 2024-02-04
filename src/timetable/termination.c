#include "termination.h"

// Should return non-zero value to terminate.
// Must ensure algo is not terminated for the very first two generation itself.
char terminationCondition(
    uint generation_number, uint population_size,
    GeneticSpecifications
        *gaSpecs) { // numeric *delta_soft_fitness, numeric *delta_hard_fitness,
                    // GeneticSpecifications *gaSpecs) {

  if (generation_number == 0 || generation_number == 1)
    return 0;

  if (generation_number > gaSpecs->maximum_generations)
    return 1;

  // Very simple termination condition for now.

  return 0;
}
