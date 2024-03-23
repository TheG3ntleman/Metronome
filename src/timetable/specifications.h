#ifndef SPECIFICATIONS_H
#define SPECIFICATIONS_H

#include "config.h"

#include <stdlib.h>
/*
typedef struct {
  uint size;
  uint *party_id;
  uint *strength;
} PartyTable;

typedef struct {
  uint size;
  uint *venue_id;
  uint *venue_type;
  uint *capacity;
  uint *number_available;
} VenueTable;

typedef struct {
  uint size;
  uint *session_id;
  uint *venue_type;
  uint *duration;
} SessionTable;

typedef struct {
  uint size;
  uint *party_id;
  uint *session_id;
  uint *priority;
} AssignmentTable;

typedef struct {
  PartyTable *parties;
  VenueTable *venues;
  SessionTable *sessions;
  AssignmentTable *assignments;
} TimeTableSpecifications;

TimeTableSpecifications *makeTimeTableSpecifications(
    // For party table
    size_t p_size, uint *p_party_id, uint *p_strength,

    // For venue table
    size_t v_size, uint *v_venue_id, uint *v_venue_type, uint *v_capacity,
    uint *v_number_available,

    // For session table
    size_t s_size, uint *s_session_id, uint *s_venue_type, uint *s_duration,

    // For assignments table
    size_t a_size, uint *a_party_id, uint *a_session_id, uint *a_priority);

void deleteTimeTableSpecifications(TimeTableSpecifications *specs);
void printTimeTableSpecificationSummary(TimeTableSpecifications *specs);*/

typedef struct {
  uint size;
  uint *id;
  uint *strength;
  uint *max_hours;
  uint *party_type;
  uint *preferred_start_time;
  uint *preferred_end_time;
  uint *preferred_back_to_back;
  uint *preferred_max_hours;
} PartyTable;

typedef struct {
  uint size;
  uint *id;
  uint *type;
  uint *capacity;
  uint *locality;
} VenueTable;

typedef struct {
  uint size;
  uint *id;
  uint *day;
} TimeSlotTable;

typedef struct {
  uint size;
  uint *id;
  uint *distance;
} LocalityTable;

typedef struct {
  uint size;
  uint *id;
  uint *type;
  uint *duration;
  uint *course;
} SessionTable;

typedef struct {
  uint size;
  uint *party_id;
  uint *session_id;
  uint *priority;
} AssignmentTable;

typedef struct {

  PartyTable *party_table;
  VenueTable *venue_table;
  TimeSlotTable *timeslot_table;
  LocalityTable *locality_table;
  SessionTable *session_table;
  AssignmentTable *assignment_table;
  float *constraint_weights;

} TimeTableSpecifications;

TimeTableSpecifications *makeTimeTableSpecifications(
    // For party table
    size_t p_size, uint *p_id, uint *p_strength, uint *p_max_hours,
    uint *p_party_type, uint *p_preferred_start_time,
    uint *p_preferred_end_time, uint *p_preferred_back_to_back,
    uint *p_preferred_max_hours,

    // For venue table
    size_t v_size, uint *v_id, uint *v_type, uint *v_capacity, uint *v_locality,

    // For timeslot table
    size_t t_size, uint *t_id, uint *t_day,

    // For locality table
    size_t l_size, uint *l_id, uint *l_distance,

    // For session table
    size_t s_size, uint *s_id, uint *s_type, uint *s_duration, uint *s_course,

    // For assignments table
    size_t a_size, uint *a_party_id, uint *a_session_id, uint *a_priority,

    // For constraint weights
    float *constraint_weights);

void deleteTimeTableSpecifications(TimeTableSpecifications *specs);
void printTimeTableSpecificationSummary(TimeTableSpecifications *specs);

void findAssociatedParties(uint session_id, uint *number_of_parties,
                           uint *party_id, TimeTableSpecifications *specs);
void findAssociatedSessions(uint party_id, uint *number_of_session,
                            uint *session_id, TimeTableSpecifications *specs);
uint locality_pair_to_index(uint locality_i, uint locality_j)
void locality_to_distance(uint locality_i, uint locality_j, uint *distance,
                          TimeTableSpecifications *specs);

#endif
