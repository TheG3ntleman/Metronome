#ifndef CULLING_H
#define CULLING_H

#include  <stdlib.h>

#include "utils.h"
#include "config.h"
#include "population.h"

#define SIMILARITY_THRESHOLD 10 // Again Random, need to change

// Structure to represent a cluster
typedef struct {
    unsigned int* schedules;  // Array of schedule indices in this cluster
    unsigned int size;        // Number of schedules in this cluster
    unsigned int capacity;    // Capacity of schedules array
} Cluster;

void cullPopulationViaClustering(Population *population, numeric *soft_fitness, numeric *hard_fitness, unsigned int selection_size, unsigned int *selection_arr,  *gaSpecs);

void cullPopulationViaSelection(Population *population, numeric *soft_fitness, numeric *hard_fitness, unsigned int selection_size, unsigned int *selection_arr, unsigned int min_schedules_per_cluster);

#endif
