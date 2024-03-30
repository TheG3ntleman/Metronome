#ifndef UTILS_H
#define UTILS_H

#define FALSE 0
#define TRUE 1

#include <stdlib.h>

typedef long double numeric;
typedef float snumeric;
typedef char bool;
typedef unsigned int uint;
// Norm functions for fitness concatination.

uint randint(uint lower, uint upper);
numeric randunif(numeric lower, numeric upper);

numeric norm_l1(numeric x1, numeric x2);
numeric norm_l2(numeric x1, numeric x2);
numeric norm_lp(numeric x1, numeric x2, uint p);
numeric norm_linfty(numeric x1, numeric x2);

typedef struct {

  uint timeslot;
  uint venue;

} TimeTableTuple;

#endif