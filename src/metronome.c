#include "metronome.h"
#include "mcts/monte_carlo_tree_search.h"
#include "src/testing/problem.h"
#include "timetable/specifications.h"

#define OPTIONS_PER_SESSION 5

void solveTimeTablingProblem(
    // For party table
    size_t p_size, uint *p_id, uint *p_strength, uint *p_max_hours,
    uint *p_party_type, uint *p_preferred_start_time,
    uint *p_preferred_end_time, uint *p_preferred_back_to_back,
    uint *p_preferred_max_hours,

    // For venue table
    size_t v_size, uint *v_id, uint *v_type, uint *v_capacity, uint *v_locality,

    // For timeslot table
    size_t t_size, uint *t_id, uint *t_day,

    // For locality table
    size_t l_size, uint *l_id, uint *l_distance,

    // For session table
    size_t s_size, uint *s_id, uint *s_type, uint *s_duration, uint *s_course,

    // For assignments table
    size_t a_size, uint *a_party_id, uint *a_session_id, uint *a_priority,

    // For constraint weights
    float *constraint_weights) {

  TimeTableSpecifications *specs = makeTimeTableSpecifications(
      // For party table
      p_size, p_id, p_strength, p_max_hours, p_party_type,
      p_preferred_start_time, p_preferred_end_time, p_preferred_back_to_back,
      p_preferred_max_hours,

      // For venue table
      v_size, v_id, v_type, v_capacity, v_locality,

      // For timeslot table
      t_size, t_id, t_day,

      // For locality table
      l_size, l_id, l_distance,

      // For session table
      s_size, s_id, s_type, s_duration, s_course,

      // For assignments table
      a_size, a_party_id, a_session_id, a_priority,

      // For constraint weights
      constraint_weights);

  printTimeTableSpecificationSummary(specs);

  // Running GA/Evolution on the timetables
  GeneticSpecifications gaSpecs = {
      .population_size = 100,
      .selection_size = 10,
      .minimum_timetables_per_cluster = 5,
      .similarity_threshold = 40,
      .mutation_rate = 0.001,
      .maximum_generations = 500,
  };

  Population *population = evolveTimeTables(specs, &gaSpecs);

  // Making an MCTS Problem from population
  MCTS_problem *problem = MCTS_make_problem_from_population(
      population, OPTIONS_PER_SESSION, 1, specs);

    MCTS_print_problem(problem);

  //MCTS_solution *solution = MCTS_execute(problem);

  deleteTimeTableSpecifications(specs);
}
