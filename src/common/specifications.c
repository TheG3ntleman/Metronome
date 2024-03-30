#include "specifications.h"
#include <stdio.h>
#include <stdlib.h>
static uint *mallocColumn(uint size) {
  uint *arr = (uint *)malloc(sizeof(uint) * size);
  return arr;
}

static void copy(uint *from, uint *to, uint size) {
  for (uint i = 0; i < size; i++) {
    to[i] = from[i];
  }
}

static uint *set(uint *src, uint size) {
  uint *res = mallocColumn(size);
  copy(src, res, size);
  return res;
}

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
    float *constraint_weights) {

  TimeTableSpecifications *specs =
      (TimeTableSpecifications *)malloc(sizeof(TimeTableSpecifications));

  // Making the party table
  PartyTable *parties = (PartyTable *)malloc(sizeof(PartyTable));

  parties->size = p_size;
  parties->id = set(p_id, p_size);
  parties->strength = set(p_strength, p_size);
  parties->max_hours = set(p_max_hours, p_size);
  parties->party_type = set(p_party_type, p_size);
  parties->preferred_start_time = set(p_preferred_start_time, p_size);
  parties->preferred_end_time = set(p_preferred_end_time, p_size);
  parties->preferred_back_to_back = set(p_preferred_back_to_back, p_size);
  parties->preferred_max_hours = set(p_preferred_max_hours, p_size);

  // Making the venue table
  VenueTable *venues = (VenueTable *)malloc(sizeof(VenueTable));

  venues->size = v_size;
  venues->id = set(v_id, v_size);
  venues->type = set(v_type, v_size);
  venues->capacity = set(v_capacity, v_size);
  venues->locality = set(v_locality, v_size);

  // Making the timeslot table
  TimeSlotTable *timeslots = (TimeSlotTable *)malloc(sizeof(TimeSlotTable));

  timeslots->size = t_size;
  timeslots->id = set(t_id, t_size);
  timeslots->day = set(t_day, t_size);

  // Making the locality table
  LocalityTable *localities = (LocalityTable *)malloc(sizeof(LocalityTable));

  localities->size = l_size;
  localities->id = set(l_id, l_size);
  localities->distance = set(l_distance, l_size);

  // Making the session table
  SessionTable *sessions = (SessionTable *)malloc(sizeof(SessionTable));

  sessions->size = s_size;
  sessions->id = set(s_id, s_size);
  sessions->type = set(s_type, s_size);
  sessions->duration = set(s_duration, s_size);
  sessions->course = set(s_course, s_size);

  // Making the assignment table
  AssignmentTable *assignments =
      (AssignmentTable *)malloc(sizeof(AssignmentTable));

  assignments->size = a_size;
  assignments->party_id = set(a_party_id, a_size);
  assignments->session_id = set(a_session_id, a_size);
  assignments->priority = set(a_priority, a_size);

  // Connecting specification with all tables
  specs->party_table = parties;
  specs->venue_table = venues;
  specs->timeslot_table = timeslots;
  specs->locality_table = localities;
  specs->session_table = sessions;
  specs->assignment_table = assignments;
  specs->constraint_weights = constraint_weights;

  return specs;
}

void deleteTimeTableSpecifications(TimeTableSpecifications *specs) {

  free(specs->party_table->id);
  free(specs->party_table->strength);
  free(specs->party_table->max_hours);
  free(specs->party_table->party_type);
  free(specs->party_table->preferred_start_time);
  free(specs->party_table->preferred_end_time);
  free(specs->party_table->preferred_back_to_back);
  free(specs->party_table->preferred_max_hours);

  free(specs->venue_table->id);
  free(specs->venue_table->type);
  free(specs->venue_table->capacity);
  free(specs->venue_table->locality);

  free(specs->timeslot_table->id);
  free(specs->timeslot_table->day);

  free(specs->locality_table->id);
  free(specs->locality_table->distance);

  free(specs->session_table->id);
  free(specs->session_table->type);
  free(specs->session_table->duration);
  free(specs->session_table->course);

  free(specs->assignment_table->party_id);
  free(specs->assignment_table->session_id);
  free(specs->assignment_table->priority);

  free(specs->party_table);
  free(specs->venue_table);
  free(specs->timeslot_table);
  free(specs->locality_table);
  free(specs->session_table);
  free(specs->assignment_table);
  // free(specs->constraint_weights);

  free(specs);
}

void printTimeTableSpecificationSummary(TimeTableSpecifications *specs) {
  printf("\nTime Table Specifications:\n");
  printf("\tThere are %u parties.\n", specs->party_table->size);
  printf("\tThere are %u venues.\n", specs->venue_table->size);
  printf("\tThere are %u timeslots.\n", specs->timeslot_table->size);
  printf("\tThere are %u localities.\n", specs->locality_table->size);
  printf("\tThere are %u sessions.\n", specs->session_table->size);
  printf("\tThere are %u assignments.\n", specs->assignment_table->size);
}

void findAssociatedParties(uint session_id, uint *number_of_parties, 
                           uint *party_id, uint depth, TimeTableSpecifications *specs) {
  *number_of_parties = 0;
  for (uint i = 0; i < specs->assignment_table->size; i++) {
    if (specs->assignment_table->session_id[i] == session_id && specs->assignment_table->session_id[i] <= depth) {
      party_id[(*number_of_parties)++] = specs->assignment_table->party_id[i];
    }
  }
}

void findAssociatedSessions(uint party_id, uint *number_of_session, 
                            uint *session_id, uint depth, TimeTableSpecifications *specs) {
  *number_of_session = 0;
  for (uint i = 0; i < specs->assignment_table->size; i++) {
    if (specs->assignment_table->party_id[i] == party_id && specs->assignment_table->session_id[i] <= depth) {
      session_id[(*number_of_session)++] =
          specs->assignment_table->session_id[i];
    }
  }
}
uint locality_pair_to_index(uint locality_i, uint locality_j, uint size) {
  if (locality_j < locality_i) {
    uint t = locality_j;
    locality_j = locality_i;
    locality_i = t;
  }

  uint c = 0, flag = 0;
  ;
  for (uint i = 0; i <= locality_i; i++) {
    for (uint j = i + 1; j <= size; j++) {
      if (i == locality_i && j == locality_j) {
        break;
        flag = 1;
      }
      c++;
    }
    if (flag)
      break;
  }
  return c;
}
void locality_to_distance(uint locality_i, uint locality_j, uint *distance,
                          TimeTableSpecifications *specs) {
  if (locality_j < locality_i) {
    uint t = locality_j;
    locality_j = locality_i;
    locality_i = t;
  } else if (locality_j == locality_i) {
    *distance = 0;
    return;
  }

  uint c = 0, flag = 0;
  ;
  for (uint i = 0; i <= locality_i; i++) {
    for (uint j = i + 1; j <= specs->locality_table->size; j++) {
      if (i == locality_i && j == locality_j) {
        break;
        flag = 1;
      }
      c++;
    }
    if (flag)
      break;
  }

  *distance = specs->locality_table->distance[c];
}
