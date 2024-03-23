#include "metronome.h"
#include "timetable/specifications.h"
#include "timetable/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "testing/problem.h"

typedef unsigned int uint;

#define MINIMUM_PARTIES 10
#define MAXIMUM_PARTIES 1000
#define MINIMUM_PARTY_STRENGTH 10
#define MAXIMUM_PARTY_STRENGTH 100

int main() {

  srand(time(NULL));

  // Sample data for demonstration purposes
  Problem *problem = make_problem();
  print_problem(problem);
  
 

  return 0;
}
