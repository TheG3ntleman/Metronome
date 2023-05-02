#ifndef TIMETABLE_H
#define TIMETABLE_H

#include "common/common.h"

typedef struct {

  uint num_parties;
  uint num_jobs;
  uint num_venues;
  char ** party_names;
  char ** job_names;
  char ** venue_names;
  uint ** job_party_requirements;
  uint * job_party_requirement_lenghts;
  uint * job_time_durations;
  uint * job_repetitions_per_cycle;
  uint * venue_capacities;

} TimeTableSpecification;

TimeTableSpecification* ttsMake();
void ttsDelete(TimeTableSpecification *tts);

void ttsAddParty(TimeTableSpecification *tts, char *party);
int ttsGetPartyIndex(TimeTableSpecification *tts, char *party);

void ttsAddJob(TimeTableSpecification *tts, char *job);
int ttsGetJobIndex(TimeTableSpecification *tts, char *job);

void ttsAddVenue(TimeTableSpecification *tts, char *venue); // NEW
int ttsGetVenueIndex(TimeTableSpecification *tts, char *job); // NEW

void ttsAddJobRequirement(TimeTableSpecification *tts, char *job, char *party);
void ttsAddJobDuration(TimeTableSpecification *tts, char *job, uint duration);
void ttsAddJobRepititions(TimeTableSpecification *tts, char *job, uint reps);

void ttsAddVenueCapacity(TimeTableSpecification *tts, char *venue, uint capacity); // NEW

void ttsPrintSpecifications(TimeTableSpecification *tts); // MUST UPDATE TO INCLUDE VENUE INFORMATION

typedef struct {

} timetable;

#endif
