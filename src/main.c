#include "common/file.h"

int main(int argc, char **argv) {

  // TODO: Argument parsing
  if (argc > 1) {
    File *file = fileOpen(argv[1]);
    printf("file contents: %s\n", file->str);
    fileClose(file);
  }
  
  return 0;
}
