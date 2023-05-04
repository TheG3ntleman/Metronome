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
  tts->num_venues = 0;

  tts->party_names = NULL;

  tts->job_names = NULL;
  tts->job_party_requirement_lenghts = NULL;
  tts->job_party_requirements = NULL;
  tts->job_time_durations = NULL;
  tts->job_repetitions_per_cycle = NULL;

  tts->venue_names = NULL;
  tts->venue_capacities = NULL;

  return tts;
}

void ttsDelete(TimeTableSpecification *tts) {

  // Deleting party names
  for (uint i = 0; i < tts->num_parties; i++) 
    stringDelete(tts->party_names[i]);
  free(tts->party_names);

  // Deleting job names
  for (uint i = 0; i <tts->num_jobs; i++) 
    stringDelete(tts->job_names[i]);
  free(tts->job_names);

  // Deleting job requirements
  for (uint i = 0; i < tts->num_jobs; i++) 
    free(tts->job_party_requirements[i]);
  free(tts->job_party_requirements);

  // Deleting job party requirement lengths
  free(tts->job_party_requirement_lenghts);

  // Deleting job time duration
  free(tts->job_time_durations);

  // Deleting job_repetitions_per_cycle
  free(tts->job_repetitions_per_cycle);

  // Deleting venue names
  for (uint i = 0; i < tts->num_venues; i++)
    stringDelete(tts->venue_names[i]);
  free(tts->venue_names);

  // Deletingh venue capacities
  free(tts->venue_capacities);

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

int ttsGetPartyIndex(TimeTableSpecification *tts, char *party) {
  uint i = 0;
  char match = 0;
  for (; i < tts->num_parties && !(match = stringCompare(tts->party_names[i], party)); i++)
    ;
  if (match)
    return i;
  else 
    return -1;
}

void ttsAddJob(TimeTableSpecification *tts, char *job) {

  // Making new job array and copying old 
  // contents as well as putting in the new name 
  char ** job_names = malloc(sizeof(char *) * (tts->num_jobs + 1));

  uint i = 0;
  for (; i < tts->num_jobs; i++)
    job_names[i] = tts->job_names[i]; 

  job_names[i] = stringDuplicate(job);

  free(tts->job_names);
  tts->job_names = job_names;

  // CAN MAKE THIS STUFF MORE CONCISE WITH A COMMON COPY FUNCTION 
  // FOR ALL SORTS OF ARRAY USING A void* CAST?

  // Making space for new job length requirement
  uint * job_party_requirement_lenghts = malloc(sizeof(uint) * (tts->num_jobs + 1));
  for (i = 0; i < tts->num_jobs; i++) {
    job_party_requirement_lenghts[i] = tts->job_party_requirement_lenghts[i];
  } 
  job_party_requirement_lenghts[i] = 0;
 
  free(tts->job_party_requirement_lenghts);
  tts->job_party_requirement_lenghts = job_party_requirement_lenghts;

  // Making space for new job party requirements
  uint ** job_party_requirements = malloc(sizeof(uint*) * (tts->num_jobs + 1));
  for (i = 0; i < tts->num_jobs; i++)
    job_party_requirements[i] = tts->job_party_requirements[i];
  job_party_requirements[i] = NULL;
  
  free(tts->job_party_requirements);
  tts->job_party_requirements = job_party_requirements;

  // Making space for job time duration
  uint * job_time_durations = malloc(sizeof(uint) * (tts->num_jobs + 1));
  for (i = 0; i < tts->num_jobs; i++) 
    job_time_durations[i] = tts->job_time_durations[i];
  job_time_durations[i] = 0;

  free(tts->job_time_durations);
  tts->job_time_durations = job_time_durations;

  // Making space for job_repetitions_per_cycle
  uint * job_repetitions_per_cycle = malloc(sizeof(uint) * (tts->num_jobs + 1));
  for (i = 0; i < tts->num_jobs; i++)
    job_repetitions_per_cycle[i] = tts->job_repetitions_per_cycle[i];
  job_repetitions_per_cycle[i] = 0;

  free(tts->job_repetitions_per_cycle);
  tts->job_repetitions_per_cycle = job_repetitions_per_cycle;

  tts->num_jobs++;


}

int ttsGetJobIndex(TimeTableSpecification *tts, char *job) {
  uint i = 0;
  char match = 0;
  for (; i < tts->num_jobs && !(match = stringCompare(tts->job_names[i], job)); i++)
    ;
  if (match)
    return i;
  else
    return -1;
}

void ttsAddVenue(TimeTableSpecification *tts, char *venue) {
  
  char **venue_names = malloc(sizeof(char*) * (tts->num_venues + 1));
  
  // copying old venue names into new list
  uint i = 0;
  for(; i < tts->num_venues; i++) 
    venue_names[i] = tts->venue_names[i];
  
  venue_names[i] = stringDuplicate(venue);

  free(tts->venue_names);
  tts->venue_names = venue_names;

  // Now doing the exact same for venue capacities
  uint *venue_capacities = malloc(sizeof(uint) * (tts->num_venues + 1));

  i = 0;
  for (; i < tts->num_venues; i++)
    venue_capacities[i] = tts->venue_capacities[i];

  venue_capacities[i] = 0;
  
  free(tts->venue_capacities);
  tts->venue_capacities = venue_capacities;

  tts->num_venues++;
}

int ttsGetVenueIndex(TimeTableSpecification *tts, char *venue) {

  char match = 0;
  uint i = 0;
  for (; i < tts->num_venues; i++) {
    if (stringCompare(tts->venue_names[i], venue)) {
      match = 1;
      break;
    }
  }

  if (match)
    return i;
  else
    return -1;

}

void ttsAddVenueCapacity(TimeTableSpecification *tts, char *venue, uint capacity) {
  int index = ttsGetVenueIndex(tts, venue);

  if (index == -1) {
    fprintf(stderr, "Reference to non-existant venue (%s).\n", venue);
    exit(-1);
  }

  tts->venue_capacities[index] = capacity;

}

void ttsAddJobRequirement(TimeTableSpecification *tts, char *job, char *party) {
  int job_index = ttsGetJobIndex(tts, job);
  if (job_index == -1) {
    fprintf(stderr, "Reference to non-existant job (%s).\n", job);
    exit(-1);
  }

  uint * job_party_requirements = tts->job_party_requirements[job_index];
  uint num_job_party_requirements = tts->job_party_requirement_lenghts[job_index];

  uint *new_job_party_requirements = malloc(sizeof(uint) * (num_job_party_requirements + 1));

  uint i = 0;
  for (; i < num_job_party_requirements; i++) 
    new_job_party_requirements[i] = job_party_requirements[i];

  int party_index = ttsGetPartyIndex(tts, party);

  if (party_index == -1) {
    fprintf(stderr, "Attempt to add non-existant party (%s) to job (%s)'s requirements\n", party, job);
    exit(-1);
  }

  new_job_party_requirements[i] = party_index; 
  
  free(job_party_requirements);
  tts->job_party_requirements[job_index] = new_job_party_requirements;

  tts->job_party_requirement_lenghts[job_index]++;
}

void ttsAddJobDuration(TimeTableSpecification *tts, char *job, uint duration) {
  int job_index = ttsGetJobIndex(tts, job);
  if (job_index == -1) {
    fprintf(stderr, "Reference to non-existant job (%s).\n", job);
    exit(-1);
  }

  tts->job_time_durations[job_index] = duration;
}
void ttsAddJobRepititions(TimeTableSpecification *tts, char *job, uint reps) {
  int job_index = ttsGetJobIndex(tts, job);
  if (job_index == -1) {
    fprintf(stderr, "Reference to non-existant job (%s).\n", job);
    exit(-1);
  }

  tts->job_repetitions_per_cycle[job_index] = reps;
}

void ttsPrintSpecifications(TimeTableSpecification *tts) {

  // Printing parties

  printf("\nPARTY NAMES: (%u parties found)\n\n", tts->num_parties);

  for (uint i = 0; i < tts->num_parties; i++) 
    printf("\t%u. %s\n", i + 1, tts->party_names[i]);
 
  // Printing jobs

  printf("\nJOBS NAMES: (%u jobs found)\n\n", tts->num_jobs);
  for (uint i = 0; i < tts->num_jobs; i++) {
    printf("\t%u. %s, %u minutes, %u reps\n", i + 1, tts->job_names[i], tts->job_time_durations[i], tts->job_repetitions_per_cycle[i]);
    uint * job_party_requirement = tts->job_party_requirements[i];
    if (job_party_requirement != NULL) {
      for (uint j = 0; j < tts->job_party_requirement_lenghts[i]; j++) {
        printf("\t\t%u. %s\n", j + 1, tts->party_names[job_party_requirement[j]]);
      }
    } else
      printf("\tNO REQUIREMENTS PROVIDED.\n");
  }

  printf("\nVENUE NAMES: (%u venues found)\n\n", tts->num_venues);
  for (uint i = 0; i < tts->num_venues; i++) {
    printf("\t%u. %s, capacity: %u\n", i + 1, tts->venue_names[i], tts->venue_capacities[i]);
  }
}


