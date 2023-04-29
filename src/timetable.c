#include "timetable.h"
#include "common/string.h"

// Implementation using dynamic arrays

/*
 *
typedef struct {

  uint num_parties;
  uint num_jobs;
  char ** party_names;
  char ** job_names;
  uint * job_party_requirement_lenghts;
  uint ** job_party_requirements;
  uint * job_time_duration;
  uint * job_repetitions_per_cycle;

} TimeTableSpecification;
 * */

TimeTableSpecification* ttsMake() {
  TimeTableSpecification *tts = malloc(sizeof(TimeTableSpecification));
  
  tts->num_parties = 0;
  tts->num_jobs = 0;

  tts->party_names = NULL;
  tts->job_names = NULL;
  tts->job_party_requirement_lenghts = NULL;
  tts->job_party_requirements = NULL;
  tts->job_time_duration = NULL;
  tts->job_repetitions_per_cycle = NULL;

  return tts;
}

void ttsDelete(TimeTableSpecification *tts) {

  // Deleting party names
  for (uint i = 0; i < tts->num_parties; i++) 
    stringDelete(tts->party_names[i]);
  free(tts->party_names);

  free(tts);
} 

void ttsAddParty(TimeTableSpecification *tts, char *party) {

  char ** party_names = malloc(sizeof(char *) * (tts->num_parties + 1));
 
  // copying old party names to the current one.
  uint i = 0;
  for (; i < tts->num_parties; i++)
    party_names[i] = tts->party_names[i];

  party_names[i] = stringDuplicate(party);

  free(tts->party_names);
  tts->party_names = party_names;
  tts->num_parties++;

}
uint ttsGetPartyIndex(TimeTableSpecification *tts, char *party) {
  uint i = 0;
  for (; !stringCompare(tts->party_names[i], party); i++)
    ;
  return i;
}

void ttsAddJob(TimeTableSpecification *tts, char *job) {

}
uint ttsGetJobIndex(TimeTableSpecification *tts, char *job) {

}

void ttsAddJobRequirement(TimeTableSpecification *tts, char *job, char *party) {

}
void ttsAddJobDuration(TimeTableSpecification *tts, char *job, uint duration) {

}
void ttsAddJobRepititions(TimeTableSpecification *tts, char *job, uint reps) {

}

void ttsPrintSpecifications(TimeTableSpecification *tts) {
  printf("PARTY NAMES: (%u parties found)\n", tts->num_parties);

  for (uint i = 0; i < tts->num_parties; i++) {
    printf("%u. %s\n", i + 1, tts->party_names[i]);
  }
}


