project('Metronome', 'C')

metronome_library = shared_library(
  'metronome',
  './src/metronome.c', 

  './src/common/utils.c', 
  './src/common/config.c', 
  './src/common/constraint.c',
  
  './src/timetable/culling.c', 
  './src/timetable/evolution.c', 
  './src/timetable/fitness.c', 
  './src/timetable/operators.c', 
  './src/timetable/population.c', 
  './src/timetable/specifications.c', 
  './src/timetable/termination.c', 
  
  './src/testing/problem.c', 

  './src/mcts/agent.c',
  './src/mcts/feasibility.c',
  './src/mcts/monte_carlo_tree_search.c',
  './src/mcts/state_space_tree.c',
  # './src/mcts/backpropagate.c',
  # './src/mcts/optimality.c',
  # './src/mcts/selection.c',
  # './src/mcts/simulation.c',

  link_args: '-lm'

)

executable('metronome', './src/main.c', link_with: metronome_library)
