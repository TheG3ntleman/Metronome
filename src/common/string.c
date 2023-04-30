#include "string.h"
#include <stdio.h>

uint stringLength(char *str) {
  uint i = 0;
  for (; str[i] != '\0'; i++)
    ;
  return i + 1; // Adding one to count '\0'
}

void stringCopy(char *to, char *from) {
  // Assumes that sufficient memory has 
  // been allocated already.
  uint i;
  for (i = 0; from[i] != '\0'; i++) {
    to[i] = from[i];
}
  to[i] = from[i];
}

char *stringDuplicate(char *str) {
  uint string_length = stringLength(str);
  char *to = malloc(sizeof(char) * string_length);

  for (uint i = 0; i < string_length; i++)
    to[i] = str[i];

  return to;
}

void stringDelete(char *str) {
  free(str);
}

char stringCompare(char *str1, char *str2) {

  uint i = 0;
 
  for (; str1[i] != '\0' && str2[i] != '\0'; i++) {
    if (str1[i] != str2[i]) 
      return 0;
  }

  if (str1[i] == '\0' && str2[i] == '\0')
    return 1;

  return 0;

}

