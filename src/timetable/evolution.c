#include "evolution.h"

Population *evolveTimeTables(TimeTableSpecifications *ttSpecs,
                             GeneticSpecifications *gaSpecs) {

  // Allocating memory for various evolutionary objects
  Population *population =
      makePopulation(gaSpecs->population_size, ttSpecs->sessions->size, 35,
                     ttSpecs->venues->size); // Timeslots are hardcoded for now.

  numeric soft_fitness[population->n_timetables];
  numeric hard_fitness[population->n_timetables];

  // Some buffer objects
  Population *new_population;
  uint indices_of_selected_timetable[gaSpecs->selection_size];

  // Initializing various evolutionary objects
  operatorInitialize(population, ttSpecs, gaSpecs);

  // Evolutionary loop
  for (uint num_generation = 1;
       !terminationCondition(num_generation, population->n_timetables, gaSpecs);
       num_generation++) {

    // Calculate fitnesses
    computeFitnesses(population, ttSpecs, soft_fitness, hard_fitness);

    // Cull Population
    cullPopulationViaSelection(population, soft_fitness, hard_fitness,
                               indices_of_selected_timetable, gaSpecs);

    // Using selected timetables to generate new population using crossover
    new_population = makePopulation(
        gaSpecs->population_size, ttSpecs->sessions->size, 35,
        ttSpecs->venues->size); // Timeslots are hardcoded for now.
    operatorCrossover(new_population, population, indices_of_selected_timetable,
                      ttSpecs, gaSpecs);

    // Deleting old population and replacing with new population
    deletePopulation(population);
    population = new_population;

    // Applying the mutation operator
    operatorMutation(population, ttSpecs, gaSpecs);

    // IO for sanity
    numeric avg_soft_fitness = 0;
    numeric avg_hard_fitness = 0;
    for (uint i = 0; i < population->n_timetables; i++) {
      avg_soft_fitness += soft_fitness[i];
      avg_hard_fitness += hard_fitness[i];
    }

    avg_soft_fitness /= population->n_timetables;
    avg_hard_fitness /= population->n_timetables;

    printf("%u. %Lf : %Lf\n", num_generation, avg_soft_fitness,
           avg_hard_fitness);
  }

  printf("Some Time Tables:\n\n");

  for (uint i = 0; i < 2; i++) {
    ttPrint(population, i);
    printf("\n\n");
  }

  return population;
}
