#include "evolution.h"
#include "fitness.h"
#include "population.h"
#include "termination.h"

// GASpecifications related functions
GASpecfications* makeGASpecifications() {
}

void deleteGASpecfications(GASpecfications *specs) {
}

// Main Evolution Function
Population* evolveTimeTables(TimeTableSpecifications *specs, GASpecfications *ga_specs) {

	// Allocating memory for necessary objects.
	Population *population = makePopulation(ga_specs->population_size);
	numeric *soft_fitness = malloc(ga_specs->population_size);
	numeric *hard_fitness = malloc(ga_specs->population_size);
	numeric *delta_soft_fitness = malloc(ga_specs->population_size);
	numeric *delta_hard_fitness = malloc(ga_specs->population_size);

	Population *new_population = makePopulation(ga_specs->population_size);
	numeric *new_soft_fitness = malloc(ga_specs->population_size);
	numeric *new_hard_fitness = malloc(ga_specs->population_size);

	// First initializing population
	initializePopulation(population);

	// Calculating inital fitnesses of organisms in population
	computeFitnesses(population, soft_fitness, hard_fitness);

	for (uint t = 0; terminationCondition(t, delta_soft_fitness, delta_hard_fitness); t++) {
		
		// First performing culling based on current fitness values
		uint *selected_organsisms = (uint*)malloc(sizeof(uint) * ga_specs->selection_size);
		cullPopulation(population, soft_fitness, hard_fitness, ga_specs->selection_size, selected_organsisms);

		// Generating new population using crossover and mutation
		crossOver(new_population, population, selected_organsisms);
		mutation(new_population);

		// Calculating fitness and updating delta fitness
		computeFitnesses(new_population, new_soft_fitness, new_hard_fitness);
		for (uint i = 0; i < ga_specs->population_size; i++) { 
			delta_soft_fitness[i] = soft_fitness[i] - new_soft_fitness[i];
			delta_hard_fitness[i] = hard_fitness[i] - new_hard_fitness[i];
		}


		// Cleaning population and fitness arrays and replacing
		deletePopulation(population);
		free(soft_fitness);
		free(hard_fitness);
		
		population = new_population;
		soft_fitness = new_soft_fitness;
		hard_fitness = new_hard_fitness;

		// Can do some basic IO here.
		numeric avg_soft_fitness = 0;
		numeric avg_hard_fitness = 0;
		for (uint i = 0; i < ga_specs->population_size; i++) {
			avg_soft_fitness = (avg_soft_fitness * i + soft_fitness[i]) / (i + 1);
			avg_hard_fitness = (avg_hard_fitness * i + hard_fitness[i]) / (i + 1);	
		}
		printf("Generation %u has a soft fitness of %f and  hard fitness of %f\n", t, avg_soft_fitness, avg_hard_fitness);

	} 

	// Deallocating objects
	// deletePopulation(population OR new_population); IS NOT DEALLOCATED BECAUSE WE MUST RETURN
	free(soft_fitness);
	free(hard_fitness);
	free(delta_soft_fitness);
	free(delta_hard_fitness);
	free(new_soft_fitness);
	free(new_hard_fitness);

	return population;
}
