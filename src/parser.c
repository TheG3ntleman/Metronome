#include "parser.h"
#include "common/string.h"

char isNumber(char *str) {
  // If each character is a digit then the str is a number.
  // floating point numbers are not numbers in .mt

  for (uint i = 0; str[i] != '\0'; i++) {
    if (!isdigit(str[i]))
      return 0;
  }
  
  return 1;
}

uint tokenLex(char * buffer, Token *tok, uint point) {
  //tokenClean(tok);
  
  tok->type = TOKEN_IDENTIFIER;
  uint i = point;
  uint wlen = 0;

  for (; buffer[i] != EOF; i++) {
    if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t') {
      if (wlen == 0 && buffer[i] == '\n') {
        tok->type = TOKEN_ENDLINE;
        stringCopy(tok->str, "<NEWLINE>");
      } else 
        tok->str[wlen] = '\0';
      break;
    }
    tok->str[wlen++] = buffer[i];
  }

  if (wlen == 0) { 
    if (buffer[i] == EOF)
      tok->type = TOKEN_EOF;
    else if (buffer[i] == '\n')
      tok->type = TOKEN_ENDLINE;
  } else if (stringCompare(tok->str, "PARTIES")) {
    tok->type = TOKEN_PARTIES;
  } else if (stringCompare(tok->str, "JOBS")) {
    tok->type = TOKEN_JOBS; 
  } else if (isNumber(tok->str)) {
    tok->type = TOKEN_NUMBERS;
  } else {
    tok->type = TOKEN_IDENTIFIER;
  }

  printf("at point: %4u found \"%10s\" of type %d\n", point, tok->str, (int)tok->type);

  if (buffer[i] == '\n') {
    if (wlen == 0)
      return i + 1;
    else 
      return i;
  } else 
    return i + 1;

}

Token *tokenMake() {
  Token *tok = malloc(sizeof(Token));
  tok->str = malloc(sizeof(char) * MAXIMUM_PARSE_WORD_LENGTH);
  tok->type = 0;
  return tok;
}

void tokenClean(Token *tok) {
  free(tok->str);
  tok->str = NULL;
  tok->type = 0;
}

void tokenDelete(Token *tok) {
  // Deleting token
  free(tok->str);
  free(tok);
}


void ttsFromBuffer(char *buffer, uint size) {
  uint i = 0; 
  Token *tok = tokenMake();

  while (i < size) {
    i = tokenLex(buffer, tok, i);
  }
}
