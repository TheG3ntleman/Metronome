#include "fitness.h"

static uint computeHardConstraint_PartyNonCollision(Population *population, TimeTableSpecifications, uint timetable_index){
  uint n = 0;
  for(i = 0; i < population->size; i++){
    getTimeTableTuple()
  }
}

static numeric computeSoftFitness(Population *population, uint timetable_index){
}

static numeric computeSoftFitness(Population *population, uint timetable_index){
}

void computeFitness(Population *population, numeric soft_fitness, numeric *hard_fitness){
  for (uint i = 0; i < population->size; i++){
    soft->fitness[i] = computeSoftFitness(population, i);
    hard_fitness[i] =  computeHardFitness(population, i);
  }
}
