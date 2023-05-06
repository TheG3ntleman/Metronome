#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include "common/common.h"
#include "timetable.h"

#define MAXIMUM_PARSE_WORD_LENGTH 150

typedef enum {

  TOKEN_IDENTIFIER,
  TOKEN_PARTIES,
  TOKEN_JOBS,
  TOKEN_VENUE,
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

TimeTableSpecification * ttsFromBuffer(char *buffer, uint size);

#endif
