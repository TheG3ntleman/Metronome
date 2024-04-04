#include "utils.h"
#include "../timetable/population.h"
#include "constraint.h"
#include "specifications.h"
#include <math.h>
#include <stdio.h>
#include <type_traits>
#define path ""
#define coef 0.2
uint randint(uint lower, uint upper) {
  return lower + rand() % (upper - lower + 1);
}

numeric randunif(numeric lower, numeric upper) {
  return lower + ((numeric)rand() / RAND_MAX) * (upper - lower);
}

// funcion to calculate absolute value of floating point number
static numeric fabsf(numeric num) {
  if (num < 0) {
    return -num;
  } else {
    return num;
  }
}

// function to calculate the power of a number
static numeric powerOptimized(numeric base, int exponent) {
  numeric result = 1;

  while (exponent > 0) {
    if (exponent % 2 == 1) {
      result *= base;
    }

    base *= base;
    exponent /= 2;
  }

  return result;
}

numeric norm_l1(numeric x1, numeric x2) { return fabsf(x1) + fabsf(x2); }

numeric norm_l2(numeric x1, numeric x2) {

  // calculating square root of x1^2+x2^2
  numeric x = x1 * x1 + x2 * x2;
  numeric epsilon = 1e-3;
  numeric guess = x / 2.0;
  while (fabsf(guess * guess - x) > epsilon) {
    guess = guess - (guess * guess - x) / (2 * guess);
  }

  return guess;
}

numeric norm_lp(numeric x1, numeric x2, uint p) {
  numeric x = powerOptimized(fabsf(x1), p) + powerOptimized(fabsf(x2), p);

  numeric epsilon = 1e-6;
  numeric guess = x / 2.0;

  // Limit the number of iterations to prevent infinite loop
  int maxIterations = 1000;

  while (fabsf(powerOptimized(guess, p) - x) > epsilon && maxIterations > 0) {
    guess = guess -
            (powerOptimized(guess, p) - x) / (p * powerOptimized(guess, p - 1));
    maxIterations--;
  }

  return guess;
}

numeric norm_linfty(numeric x1, numeric x2) {
  // returning the bigger absolute value
  if (fabsf(x1) > fabsf(x2)) {
    return x1;
  } else {
    return x2;
  }
}

uint Wrapper_Constraints_test(uint (*function)(), TimeTableTuple *timetable,
                              TimeTableSpecifications *specs, uint depth) {

  uint number_of_violations = function(*timetable, *specs, depth);
  // printf("Function name: %s\n", __func__);
  printf("The number of violations %d\n", number_of_violations);
  return number_of_violations;
}

void Wrapper_Constraints(Population population, TimeTableSpecifications *specs,
                         uint depth) {
  for (int i = 0; i < population.n_timetables; i++) {
    TimeTableTuple temp = {i, i};

    uint (*functions[])() = {hard_party_conflict,
                             hard_repeated_tuple,
                             hard_venue_capacity,
                             hard_venue_type,
                             hard_max_hours,
                             hard_multi_timeslot,
                             soft_travel_time,
                             soft_chunking,
                             soft_room_utilization,
                             soft_extreme_time,
                             soft_room_capacity_utilization,
                             soft_common_timeslot_empty,
                             soft_minimize_back_to_back,
                             soft_repeated_course_session,
                             soft_sessions_well_distributed,
                             soft_lab_after_lecture};

    uint size = sizeof(functions) / sizeof(functions[0]);
    for (uint i = 0; i < size; i++) {
      uint val = Wrapper_Constraints_test(functions[i], &temp, specs, depth);

      // uint val = Wrapper_Constraints_test(function_ptr, &temp, specs, depth);
      // path is defined at the top
      FILE *csvFile = fopen(path, "a");
      if (csvFile == NULL) {
        printf("Failed to open the CSV file.\n");
        return;
      }

      fprintf(csvFile, "%d,", val);
      fclose(csvFile);

      if (i == population.n_timetables - 1) {
        fprintf(csvFile, "\n");
        fclose(csvFile);
      }
    }
  }
}