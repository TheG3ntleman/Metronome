#include "termination.h"
#include "population.h"

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

char terminationCondition_temp(uint generation_number, uint population_size,
                               numeric *delta_soft_fitness,
                               numeric *delta_hard_fitness,
                               GeneticSpecifications *gaSpecs,
                               Population *population) {

  // We will calculate the similarity, betweeen the time tables, if the
  // cummulaitive similarity is less than the threshold, we can terminate
  numeric accumulated_similarity = 0;
  for (uint i = 0; i < population->n_timetables; i++) {
    for (uint j = i + 1; j < population->n_timetables; j++) {
      numeric similarity = ttCalculateSimilarity(population, i, j);
      accumulated_similarity += similarity;
      if(accumulated_similarity < gaSpecs->similarity_threshold){
        return 1;
        // Indicating similar time tables, cause of the accumulated similarity
      }
    }
  }
  return 0;
}
