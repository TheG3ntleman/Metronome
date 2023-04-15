#include <stdio.h>
#include <stdlib.h>
#include "common.h"

typedef enum  {
  
  FILE_ARGUMENT,
  CHARACTER_ARGUMENT,
  STRING_ARGUMENT

} argType;

int main(int argc, char **argv) {

  // For now the command line parser will only accept
  // input file paths
  
  for (uint i = 1; i < argc; i++) {
    argType argumentType = FILE_ARGUMENT;
    uint wordStart = 0;
    for (uint j = 0; argv[i][j] != '\0'; j++) {
    
      if (j == 0 && argv[i][j] == '-') {
        argumentType = CHARACTER_ARGUMENT; 
      } else if (j == 1 && argv[i][j] == '-') {
        argumentType = STRING_ARGUMENT;
      } else {
        wordStart = 0;
        break;
      }
    
    }

    switch (argumentType) {
      case FILE_ARGUMENT:
        printf("Loading: %s\n", argv[i] + wordStart);
        // Load specification file and send it to parser.
        FILE *specifcationFile = fopen(argv[i] + wordStart, "r");

        if (specifcationFile == NULL) {
          printf("Could not load file: %s\n", argv[i] + wordStart);
          return 0;
        }

        fseek(specifcationFile, 0L, SEEK_END);
        unsigned long fsize = ftell(specifcationFile);
        rewind(specifcationFile);
        
        // Handle case where you fail to allocate memory.
        char *buffer = (char *)malloc(fsize + 1);

        if (1 != fread(buffer, fsize, 1, specifcationFile)) {
          fprintf(stderr, "Failed to read entire shader file: %s\n", argv[i] + wordStart);
          fclose(specifcationFile);
          free(buffer);
          exit(1);
        }

        fclose(specifcationFile);

        // You can use the buffer contents here.

        free(buffer);
         
        break;
      case CHARACTER_ARGUMENT:
        break;
      case STRING_ARGUMENT:
        break;
    };

  }

  return 0;

}

