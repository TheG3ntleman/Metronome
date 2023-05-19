#include "parser.h"
#include "common/string.h"
#include "timetable.h"

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
  } else if (stringCompare(tok->str, "VENUES")) {
    tok->type = TOKEN_VENUE;
  }else if (isNumber(tok->str)) {
    tok->type = TOKEN_NUMBERS;
  } else {
    tok->type = TOKEN_IDENTIFIER;
  }

  //printf("at point: %4u found \"%10s\" of type %d\n", point, tok->str, (int)tok->type);

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

TimeTableSpecification * ttsFromBuffer(char *buffer, uint size) {
  uint i = 0; 
  Token *tok = tokenMake();

  TimeTableSpecification *tts = ttsMake();

  // 0 stands for searching for a
  // command mode.
  // 1 stands for scanning for 
  // PARTY names 
  char mode = 0;

  char job_mode = 0;
  char *job_name;

  char venue_mode = 0;
  char *venue_name;

  while (i < size) {
    i = tokenLex(buffer, tok, i);

    if (tok->type == TOKEN_PARTIES) {
      mode = 1;
    } else if (tok->type == TOKEN_JOBS) {
      mode = 2;
      job_mode = 0;
    } else if (tok->type == TOKEN_VENUE) {
      mode = 3;
    }else {
      if (mode == 0) {
        printf("Found identifier \"%s\" without receiving a scanning mode command.\n", tok->str);
        exit(0);
      } else if (mode == 1){
        if (tok->type == TOKEN_IDENTIFIER) {
          ttsAddParty(tts, tok->str);
        } else if (tok->type == TOKEN_NUMBERS) {
          printf("Found number during parties scanning mode.\n");
        } else if (tok->type == TOKEN_ENDLINE) {

        } else if (tok->type == TOKEN_EOF) {

        }
      } else if (mode == 2) {
        if (tok->type == TOKEN_IDENTIFIER) {
          if (job_mode == 0) {
            ttsAddJob(tts, tok->str);
            job_name = stringDuplicate(tok->str);
            job_mode = 1;
          } else if (job_mode == 1) {
            ttsAddJobRequirement(tts, job_name, tok->str);
          }
        } else if (tok->type == TOKEN_NUMBERS) {
          if (job_mode == 1) {
            job_mode = 2;
            ttsAddJobDuration(tts, job_name, atoi(tok->str));
          } else if (job_mode == 2) {
            job_mode = 3;
            ttsAddJobRepititions(tts, job_name, atoi(tok->str));
            stringDelete(job_name);
          }
        } else if (tok->type == TOKEN_ENDLINE) {
          if (job_mode != 3 && job_mode != 0) {
            printf("Job specfication incomplete.\n");
            exit(0);
          }
          job_mode = 0;
        } else if (tok->type == TOKEN_EOF) {

        }
      } else if (mode == 3) {
        if (tok->type == TOKEN_IDENTIFIER) {
          if (venue_mode == 0) {
            ttsAddVenue(tts, tok->str);
            venue_name = stringDuplicate(tok->str);
            venue_mode = 1;
          } 
        } else if (tok->type == TOKEN_NUMBERS) {
          if (venue_mode == 1) {
            ttsAddVenueCapacity(tts, venue_name, atoi(tok->str));
            venue_mode = 0;
            stringDelete(venue_name);
          }
        }
      }
    }

  }

  tokenDelete(tok);
  return tts;
}
