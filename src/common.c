#include "common.h"

// String manipulation stuff

char isNumber(char *str) {
  // If each character is a digit then the str is a number.
  // floating point numbers are not numbers in .mt

  for (uint i = 0; str[i] != '\0'; i++) {
    if (!isdigit(str[i]))
      return 0;
  }
  
  return 1;
}

char stringCompare(char *str_a, char *str_b) {
  uint i;

  for (i = 0; str_a[i] != '\0'; i++) {
    if (str_a[i] != str_b[i])
      return 0;
  }

  if (str_b[i] != '\0')
    return 0;

  return 1;
}

uint stringLength(string str) {
  uint i = 0; 
  for (; str[i] != '\0'; i++)
    ;
  return i;
}

string stringDuplicate(string str) {

  uint length = stringLength(str);
  string duplicate = malloc(sizeof(char) * length);

  for (uint i = 0; i < length; i++)
    duplicate[i] = str[i];

  return duplicate;
}

void stringDelete(string str) {
  free(str);
}

// LINKED LIST STUFF

// STRING SPECIFIC IMPLMENTATION


StrList* makeStringList() {
  StrList *list = malloc(sizeof(StrList));
  list->next = NULL;
  return list;
}

void deleteStringList(StrList *list) {
  StrList *curr_node, *next_node;
  for (curr_node = list; curr_node->next != NULL; curr_node = next_node) {
    next_node = curr_node->next;
    stringDelete(curr_node->str);
    free(curr_node);
  }
}

uint getLengthList(StrList *list) {
  StrList *curr_node = list;
  uint i = 0;
  for (; curr_node->next != NULL; i++, curr_node = curr_node->next)
    ;
  return i;
}

void addStringToList(StrList *list, string str) {
  StrList *new_node = malloc(sizeof(StrList));
  new_node->next = NULL;
  StrList *curr_node = list;
  for (; curr_node->next != NULL; curr_node = curr_node->next);
  curr_node->next = new_node;
  curr_node->str = stringDuplicate(str);
}

void printStringList(StrList *list){
  uint i = 0;
  StrList *curr_node = list;
  for (; curr_node->next != NULL; curr_node = curr_node->next, i++) {
    printf("%u: %p. str:%s next:%p\n", i, curr_node, curr_node->str, curr_node->next);
  }
  printf("%u: %p. str:%s next:%p\n", i, curr_node, curr_node->str, curr_node->next);
}

string getElementList(StrList *list, uint n) {
  StrList *curr_node = list;
  // Didn't handle the error here possible overflow.
  for (uint i = 0; i < n; i++, curr_node = curr_node->next);
  return curr_node->str;
}

