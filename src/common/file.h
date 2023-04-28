#ifndef FILE_H
#define FILE_H

#include "common.h"

typedef struct {
  FILE* obj;
  ulong size;
  string str;
} File;

File* fileOpen(string file_path);
void fileClose(File* file);

#endif
