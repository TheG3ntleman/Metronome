#include "operators.h"

// Function to initialize population randomly
void operatorInitialize(Population *population,
                        TimeTableSpecifications *ttSpecs,
                        GeneticSpecifications *gaSpecs) {

  // Running through all time tables
  for (uint t = 0; t < population->n_timetables; t++) {
    // Iterating through all sessions
    for (uint s = 0; s < population->n_sessions; s++) {
      // For now selecting a number from 0-35.
      // Later select a number from timeslot_id in the
      // timeslot table of the tt specifications.
      uint timeslot_id = randint(0, 34);
      uint venue_id =
          ttSpecs->venue_table->id[randint(0, ttSpecs->venue_table->size - 1)];

      ttSetTuple(population, t, s, timeslot_id, venue_id);
    }
  }
}

// Function for crossover operator
void operatorCrossover(Population *new_population,
                       Population *current_population,
                       uint *indices_of_selected_organisms,
                       TimeTableSpecifications *ttSpecs,
                       GeneticSpecifications *gaSpecs) {

  // Iterating through the size of the population
  // to generate a sufficient number of timetables for
  // the next population.

  for (uint t_i = 0; t_i < current_population->n_timetables; t_i++) {

    // Now randomly selecting 2 of the selected time-
    // tables returned by the culling mechanism

    uint s_id_1 =
        indices_of_selected_organisms[rand() % gaSpecs->selection_size];
    uint s_id_2;

    do {
      s_id_2 = indices_of_selected_organisms[rand() % gaSpecs->selection_size];
    } while (s_id_2 == s_id_1);

    // Now performaing a tuple-wise random crossover between these two
    // timetables

    for (uint s = 0; s < current_population->n_sessions; s++) {
      uint coin_toss = rand() % 2;
      uint timeslot_id, venue_id;
      if (coin_toss)
        ttGetTuple(current_population, s_id_1, s, &timeslot_id, &venue_id);
      else
        ttGetTuple(current_population, s_id_2, s, &timeslot_id, &venue_id);
      ttSetTuple(new_population, t_i, s, timeslot_id, venue_id);
    }
  }
}

// Function for mutation operator
void operatorMutation(Population *population, TimeTableSpecifications *ttSpecs,
                      GeneticSpecifications *gaSpecs) {

  // Iterating through all timetables
  for (uint t_i = 0; t_i < population->n_timetables; t_i++) {
    // Iterating through all sessions
    for (uint s = 0; s < population->n_sessions; s++) {
      numeric r = randunif(0, 1);
      if (r < gaSpecs->mutation_rate) {
        uint timeslot_id = randint(0, 34);
        ttSetTimeslot(population, t_i, s, timeslot_id);
      }

      r = randunif(0, 1);
      if (r < gaSpecs->mutation_rate) {
        uint venue_id = ttSpecs->venue_table
                            ->id[randint(0, ttSpecs->venue_table->size - 1)];
        ttSetVenue(population, t_i, s, venue_id);
      }
    }
  }
}
