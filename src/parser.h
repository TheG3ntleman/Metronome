#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include "common/common.h"

#define MAXIMUM_PARSE_WORD_LENGTH 150

typedef enum {

  TOKEN_IDENTIFIER,
  TOKEN_PARTIES,
  TOKEN_JOBS,
  TOKEN_NUMBERS,
  TOKEN_EOF,
  TOKEN_ENDLINE

} TokenType;

typedef struct {
  
  char * str;
  TokenType type;
  //uint wlen;

} Token;

uint tokenLex(char * buffer, Token *tok, uint point);
Token *tokenMake();
void tokenClean(Token *tok);
void tokenDelete(Token *tok);

void ttsFromBuffer(char *buffer, uint size);

#endif
