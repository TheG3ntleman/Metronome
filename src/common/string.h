#ifndef STRING_H

#include <stdlib.h>

typedef unsigned int uint;

uint stringLength(char *str);
char *stringDuplicate(char *str);
void stringCopy(char *to, char *from);
void stringDelete(char *str);
char stringCompare(char *str1, char *str2);

#endif
