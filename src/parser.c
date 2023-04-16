#include "parser.h"
#include <string.h>
#include <ctype.h>

#define MAX_PARSE_WORD_LENGTH 100

typedef enum {
  IDENTIFIER,
  TOKEN_PARTIES,
  TOKEN_JOBS,
  TOKEN_NUMBER
} TokenType;

typedef struct  {
  char str[MAX_PARSE_WORD_LENGTH]; 
  TokenType type;
  uint wlen;
} Token;

char isnumber(char *str) {
  for (uint i = 0; str[i] != '\0'; i++) {
    if (!isdigit(str[i]))
      return 0;
  }
  return 1;
}

uint lexer(char *buffer, uint startpoint, uint size, Token *token) {
  
  token->wlen = 0;
  uint i = startpoint;

  for (; i < size + 2; i++) {
    if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == EOF) {
      if (token->wlen > 0) {
        token->str[token->wlen] = '\0';
        if (strcmp(token->str, "PARTIES")) 
          token->type = TOKEN_PARTIES;
        else if (strcmp(token->str, "JOBS"))
          token->type = TOKEN_JOBS;
        else if (isnumber(token->str)){
          token->type = TOKEN_NUMBER;
        } else {
          token->type = IDENTIFIER;
        }
        return i + 1;
      }
    } else {
      token->str[token->wlen++] = buffer[i];
    }
  } 
}

void makeTimeTableSpecification(char *buffer, uint size) {
  Token token;
  for (uint i = 0; i < size - 1;) {
    i = lexer(buffer, i, size, &token);
    printf("token_string: %s, token_length: %u, token_type: %d, point: %u\n", token.str, token.wlen, token.type, i);
  } 
}

void deleteTimeTableSpecification(TimeTableSpecification *time_table_specification) {

}


