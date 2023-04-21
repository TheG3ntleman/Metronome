#ifndef COMMON_H
#define COMMON_H

// Compiling settings

// Debug settings
#define DEBUG
#define MAX_DEBUG_STRING_FORMATTING_LENGTH 20

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef unsigned int uint;

// Need to add string manipulation stuff

typedef char* string;

char isNumber(string str);
char stringCompare(string str_a, string str_b);
uint stringLength(string str);
string stringDuplicate(string str);
void stringDelete(string str);

// Need to add linked list stuff;
// 2 linked list implementations will be made here
// one specifically to store strings and another to 
// store more general void* data.

// STRING IMPLEMENTATION
typedef struct StrList {
  
  string str;
  struct StrList *next;

}StrList;

StrList* makeStringList();
void deleteStringList(StrList *list);
uint getLengthList(StrList *list);
string getElementList(StrList *list, uint n);
void addStringToList(StrList *list, string str);

void printStringList(StrList *list);


// Need to add hashmap stuff
// 2 linked list implementations will be made here
// one specifically to map strings to indices (and back) 
// and another to store more general void* data.

#endif
