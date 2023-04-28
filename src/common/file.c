
#include "file.h"


File* fileOpen(string file_path) {
  File *file = malloc(sizeof(File));

  // loading file
  file->obj = fopen(file_path, "r+");
  
  if (file->obj == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", file_path);
    exit(1);
  }

  // Checking size of file contents to dynamically
  // allocate space for appropriate buffer.
  fseek(file->obj, 0L, SEEK_END);
  file->size = ftell(file->obj);

  file->str = malloc(file->size + 1);
  if (file->str == NULL) {
    fprintf(stderr, "Failed to allocate space for file buffer: %s\n", file_path);
    exit(1);
  }

  // Loading contents of file into 
  // buffer

  rewind(file->obj);
  if (1 != fread(file->str, file->size, 1, file->obj)) {
    fprintf(stderr, "Failed to copy file into buffer: %s\n", file_path);
    free(file->str);
    fclose(file->obj);
    exit(1);
  }

  return file; 

}

void fileClose(File* file) {
  fclose(file->obj);
  free(file->str);
  free(file);
}

