#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

typedef float numeric;
typedef unsigned int uint;

// A bunch of norm options
numeric norm_l1(numeric x1, numeric x2);
numeric norm_l2(numeric x1, numeric x2);
numeric norm_lp(numeric x1, numeric x2, uint p);
numeric norm_linfty(numeric x1, numeric x2);

#endif
