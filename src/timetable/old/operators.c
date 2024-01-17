#include "operators.h"
#include <stdlib.h>

#define MAX_NUMBER_VENUES 10
#define MAX_NUMBER_TIME_SLOTS 24
#define SCHEDULE_LENGTH 5 // Example schedule length

// Make constructor and destructor for this object, info can be found in population struct
typedef struct {
    int venues[SCHEDULE_LENGTH];
    int time_slots[SCHEDULE_LENGTH];
} Schedule;

// 
typedef struct {
    unsigned int number_of_organisms;
    Schedule *organisms;
} Population;

void initializeSchedule(Schedule *schedule) {
	// Call setters
    for (int i = 0; i < SCHEDULE_LENGTH; ++i) {
	// Extract  venue ids from time table specification object, take it as argument.
        schedule->venues[i] = rand() % MAX_NUMBER_VENUES;       // Example initialization for venue (range 0-MAX_NUMBER_VENUES)
        schedule->time_slots[i] = rand() % MAX_NUMBER_TIME_SLOTS;  // Example initialization for time slot (range 0-MAX_NUMBER_TIME_SLOTS)
    }
}

void initializePopulation(Population *population) {
	// make population function
    population->organisms = (Schedule *)malloc(population->n_organisms * sizeof(Schedule));

	
    for (unsigned int i = 0; i < population->n_organisms; ++i) {
        initializeSchedule(&population->organisms[i]);
    }
}

Population *crossOver(Population *newPopulation, Population *population, unsigned int *selected_organisms) {
	// make population function can be user here.
    newPopulation->number_of_organisms = population->number_of_organisms;
    newPopulation->organisms = (Schedule *)malloc(newPopulation->number_of_organisms * sizeof(Schedule));

    for (unsigned int i = 0; i < newPopulation->number_of_organisms; ++i) {
		// Can be changed to some other policy
        Schedule parent1 = population->organisms[selected_organisms[i]];
        Schedule parent2 = population->organisms[selected_organisms[(i + 1) % population->number_of_organisms]];  // Selecting next organism as the second parent

        Schedule child;

        // Swap venues and time slots based on random choice
        for (int j = 0; j < SCHEDULE_LENGTH; ++j) {
			// Put an if, to put one or the other tuple completely.
            child.venues[j] = rand() % 2 ? parent1.venues[j] : parent2.venues[j];
            child.time_slots[j] = rand() % 2 ? parent1.time_slots[j] : parent2.time_slots[j];
        }

        newPopulation->organisms[i] = child;
    }

    return newPopulation;
}

void mutation(Population *population) {
    for (unsigned int i = 0; i < population->number_of_organisms; ++i) {
        Schedule *schedule = &population->organisms[i];

        // Perturbation done on even and odd values, 1:1 ratio for each venue and time slot
        for (int j = 0; j < SCHEDULE_LENGTH; ++j) {
            if (rand() % 2) {
                schedule->venues[j] = rand() % 10;       // Perturb venue
            }
            if (rand() % 2) {
                schedule->time_slots[j] = rand() % 24;   // Perturb time slot
            }
        }
    }
}
