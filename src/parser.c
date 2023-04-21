#include "parser.h"
#include "common.h"

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
        if (stringCompare(token->str, "PARTIES")) 
          token->type = TOKEN_PARTIES;
        else if (stringCompare(token->str, "JOBS"))
          token->type = TOKEN_JOBS;
        else if (isNumber(token->str)){
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

int getStringIndex(char **strList, uint listLength, char *str) {
  for (uint i = 0; i < listLength; i++) {
    if (stringCompare(strList[i], str)) {
      return i;
    }
  }
  return -1;
}

void makeTimeTableSpecification(char *buffer, uint size) {
  Token token;
  
#ifdef DEBUG
  for (uint i = 0; i < size - 1;) {
    i = lexer(buffer, i, size, &token);
    printf("token_string: %20s, token_length: %u, token_type: %d, point: %u\n", token.str, token.wlen, token.type, i);
  }
#endif

  // state stuff
  char mode = 0; // 0 for none
  // 1 for parties
  // 2 for jobs
  char mode_1_1 = 0;
  uint mode_1_1_num = 0;

  char mode_2_1 = 0;
  uint mode_2_1_num = 0;


  TimeTableSpecification *tts = malloc(sizeof(TimeTableSpecification));

  // initializing tts
  tts->num_parties = 0;
  tts->num_jobs = 0;
  tts->party_names = malloc(sizeof(char*) * 100); // can store a maximum of 100 parties

  StrList *party_names = makeStringList();  
  StrList *job_names = makeStringList();

  for (uint i = 0; i < size -1;) {
    i = lexer(buffer, i, size, &token);
    printf("%u: %u\n", i, size);

    // Token.type handling mode
    switch (token.type) {
      case TOKEN_PARTIES:
        mode = 1;
        break;
      case TOKEN_JOBS:
        mode = 2;
        tts->job_parties_needed = malloc(sizeof(uint *) * tts->num_jobs);
        tts->job_time_duration = malloc(sizeof(uint) * tts->num_jobs);
        tts->job_occurances = malloc(sizeof(uint) * tts->num_jobs);
        break;
    };

    if (mode == 1 && token.type == TOKEN_NUMBER && !mode_1_1) {
      mode_1_1 = 1;
      tts->num_parties = atoi(token.str);
      tts->party_names = malloc(sizeof(char *) * tts->num_parties);
    }
    if (mode == 1 && token.type == IDENTIFIER && mode_1_1) {
      addStringToList(party_names, token.str);
      tts->party_names[mode_1_1_num++] = stringDuplicate(token.str);
    }

    if (mode_1_1_num > tts->num_parties) {
      mode = 0;
    }

    if (mode == 2 && token.type == TOKEN_NUMBER && !mode_2_1) {
      mode_2_1 = 1;
      tts->num_jobs = atoi(token.str);
      tts->job_names = malloc(sizeof(char *) * tts->num_parties);
    }

    if (mode == 2 && token.type == IDENTIFIER && mode_2_1 == 1) {
      // need some uint linked list stuff here.
      tts->job_names[mode_2_1_num++] = stringDuplicate(token.str);
      mode_2_1 = 2;
    }

    if (mode == 2 && token.type == IDENTIFIER && mode_2_1 == 2) {
      addStringToList(job_names, token.str);
    }
 
    if (mode == 2 && token.type == TOKEN_NUMBER && mode_2_1 == 3) {
      tts->job_occurances[mode_2_1_num - 1] = atoi(token.str);
      mode_2_1 = 1;
    }  

    if (mode == 2 && token.type == TOKEN_NUMBER && mode_2_1 == 2) {
      printf("lengthSD: %u, mode_num: %u\n", getLengthList(job_names), mode_2_1_num);
      tts->job_parties_needed[mode_2_1_num - 1] = malloc(sizeof(uint*) * getLengthList(job_names) + 1);
      for (uint j = 0; j < getLengthList(job_names); j++) {
        tts->job_parties_needed[mode_2_1_num - 1][j] = 2 + getStringIndex(tts->party_names, tts->num_parties, getElementList(job_names, j));
      }
      tts->job_parties_needed[mode_2_1_num - 1][getLengthList(job_names)] = 0; 
      tts->job_time_duration[mode_2_1_num - 1] = atoi(token.str); 
      mode_2_1 = 3;
      printf("SAME\n");
    }


    if (mode_2_1_num > tts->num_jobs) {
      mode = 0;
    }
    
  }

  printf("PRINTING STORED PARTY NAMES\n");
  printStringList(party_names); 

  for (uint i = 0; i < tts->num_parties; i++) 
    printf("%u. %s\n", i, tts->party_names[i]);     


  printf("PRINTING JOB NAMES AND DETAILS\n");
  for (uint i = 0; i <tts->num_jobs; i++) {
    printf("%s\n", tts->job_names[i]);
    printf("PARTY NEEDED, thing: %u:\n", tts->job_parties_needed[i][0]);
    for (uint j = 0; tts->job_parties_needed[i][j] != 0; j++) {
      printf("%s\n", tts->party_names[tts->job_parties_needed[i][j] - 2]);
    }
  }
   

}

void deleteTimeTableSpecification(TimeTableSpecification *time_table_specification) {

}


