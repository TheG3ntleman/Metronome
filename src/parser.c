#include "parser.h"
#include <ctype.h>

#define MAX_PARSE_WORD_LENGTH 100

typedef enum {
  IDENTIFIER,
  TOKEN_PARTIES,
  TOKEN_JOBS,
  TOKEN_NUMBER,
  TOKEN_END,
  TOKEN_EOF
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

char strcomp(char *str_a, char *str_b) {
  uint i;
  for (i = 0; str_a[i] != '\0'; i++) {
    if (str_a[i] != str_b[i])
      return 0;
  }

  if (str_b[i] != '\0')
    return 0;

  return 1;
}

uint lexer(char *buffer, uint startpoint, uint size, Token *token) {
  
  token->wlen = 0;
  uint i = startpoint;

  for (; i < size + 1; i++) {
    if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == EOF || buffer[i] == ';') {
      if (token->wlen > 0) {
        token->str[token->wlen] = '\0';
        uint jump =  1;
        if (buffer[i] == ';') {
          jump = 0;
        }
        if (strcomp(token->str, "PARTIES")) 
          token->type = TOKEN_PARTIES;
        else if (strcomp(token->str, "JOBS"))
          token->type = TOKEN_JOBS;
        else if (isnumber(token->str)){
          token->type = TOKEN_NUMBER;
        } else {
          token->type = IDENTIFIER;
        }
        return i + jump;
      }
      if (token->wlen == 0 && buffer[i] == ';') {
        token->str[token->wlen++] = ';';
        token->str[token->wlen] = '\0';
        token->type = TOKEN_END;
        return i + 1;
      }
    } else {
      token->str[token->wlen++] = buffer[i];
    }
  }

  token->type = TOKEN_EOF;
  return size;
}

void makeTimeTableSpecification(char *buffer, uint size) {
  Token token;
  
#ifdef DEBUG
  for (uint i = 0; i < size - 1;) {
    i = lexer(buffer, i, size, &token);
    printf("token_string: %20s, token_length: %u, token_type: %d, point: %u\n", token.str, token.wlen, token.type, i);
  }
#endif

}

void deleteTimeTableSpecification(TimeTableSpecification *time_table_specification) {

}


