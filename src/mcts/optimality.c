#include "optimality.h"

numeric soft_constraint_student_travel_time(TimeTableEntry *timetable,
                                            uint n_sessions,
                                            TimeTableSpecifications *specs) {
  /*Consecutive sessions may need to be scheduled in close proximity to each
   * other to minimize student travel time between them.*/
  numeric violations = 0;

  for (uint i = 0; i < specs->party_table->size; i++) {

    uint session_id_array[specs->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array, specs);

    uint timeslot_array[specs->timeslot_table->size];
    uint venue_array[specs->venue_table->size];
    uint n = 0;

    for (uint j = 0; j < number_of_session; j++) {
      // timetable[i].timeslot
      uint venue_id = timetable[session_id_array[j]].venue;
      uint timeslot_id = timetable[session_id_array[j]].timeslot;
      timeslot_array[n] = timeslot_id;
      venue_array[n] = venue_id;
      n++;
    }

    for (uint k = 1; k < number_of_session; k++) {
      uint key_timeslot = timeslot_array[k];
      uint key_session_id = session_id_array[k];
      uint key_venue = venue_array[k];
      uint l = k - 1;

      while (l >= 0 && timeslot_array[l] > key_timeslot) {
        // Directly swap elements
        timeslot_array[l + 1] = timeslot_array[l];
        session_id_array[l + 1] = session_id_array[l];
        venue_array[l + 1] = venue_array[l];
        l = l - 1;
      }

      // Assign the key values to their correct positions
      timeslot_array[l + 1] = key_timeslot;
      session_id_array[l + 1] = key_session_id;
      venue_array[l + 1] = key_venue;
    }

    for (uint j = 0; j < n - 1; j++) {
      uint day1 = specs->timeslot_table->day[timeslot_array[j]];
      uint day2 = specs->timeslot_table->day[timeslot_array[j + 1]];
      if (day1 == day2) {
        uint locality_i = specs->venue_table->locality[venue_array[j]];
        uint locality_j = specs->venue_table->locality[venue_array[j + 1]];
        uint distance = 0;
        locality_to_distance(locality_i, locality_j, &distance, specs);
        violations += distance;
      }
    }
  }
  // find a better way to convert this quantity into a maximization problem
  return specs->constraint_weights[0] / violations;
}

numeric soft_constraint_maximize_chunking(TimeTableEntry *timetable,
                                          uint n_sessions,
                                          TimeTableSpecifications *specs) {

  /* Maximize session chunking (have back-to-back sessions for students). */
  numeric violations = 0;

  for (uint i = 0; i < specs->party_table->size; i++) {
    uint session_id_array[specs->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array, specs);
    uint timeslot_array[specs->timeslot_table->size];
    for (uint j = 0; j < specs->timeslot_table->size; j++) {
      timeslot_array[j] = 0;
    }
    uint n = 0;
    for (uint j = 0; j < number_of_session; j++) {
      uint venue_id = timetable[session_id_array[j]].venue;
      uint timeslot_id = timetable[session_id_array[j]].timeslot;
      timeslot_array[n] = timeslot_id;
      n++;
    }
    for (uint j = 0; j < n - 1;
         j++) { // Loop up to n - 1 to avoid out-of-bounds access
      if (timeslot_array[j] + 1 != timeslot_array[j + 1]) {
        violations++;
      }
    }
  }

  return specs->constraint_weights[1] / violations;
}

numeric soft_constraint_room_utlisation(TimeTableEntry *timetable,
                                        uint n_sessions,
                                        TimeTableSpecifications *specs) {

  /* Maximize Room Utilization Constraint: Encourage the efficient use of rooms
   by maximizing their overall utilization. */
  numeric violations = 0;

  uint room_utilization[specs->venue_table->size];
  for (uint i = 0; i < specs->venue_table->size; i++) {
    room_utilization[i] = 0;
  }

  for (uint i = 0; i < n_sessions; i++) { // Corrected n_sesions to n_sessions
    uint timeslot_id = timetable[i].timeslot;
    uint venue_id = timetable[i].venue;
    room_utilization[venue_id]++;
  }
  for (uint i = 0; i < specs->venue_table->size;
       i++) { // Corrected venue_table to venue_table->size
    if (room_utilization[i] == 0) {
      violations++;
    }
  }

  return specs->constraint_weights[8] / violations;
}

numeric soft_constraint_avoid_early_time(TimeTableEntry *timetable,
                                         uint n_sessions,
                                         TimeTableSpecifications *specs) {

  /* Avoid Early/Late Times Constraint: Express preferences to avoid scheduling
   * classes too early or too late in the day. */
  numeric violations = 0;
  uint number_of_days =
      specs->timeslot_table->day[specs->timeslot_table->size - 1];
  uint number_of_timeslot_per_day =
      specs->timeslot_table->size / number_of_days;

  for (uint i = 0; i < specs->party_table->size; i++) {

    uint session_id_array[specs->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array, specs);
    uint preferred_start_time = specs->party_table->preferred_start_time[i];
    uint preferred_end_time = specs->party_table->preferred_end_time[i];

    for (uint j = 0; j < number_of_session; j++) {

      uint timeslot_id = timetable[session_id_array[j]].timeslot;
      uint venue_id = timetable[session_id_array[j]].venue;

      if ((timeslot_id / number_of_timeslot_per_day) < preferred_start_time ||
          (timeslot_id / number_of_timeslot_per_day) > preferred_end_time) {
        violations++;
      }
    }
  }

  return specs->constraint_weights[9] / violations;
}

numeric
soft_constraint_room_capacity_utilisation(TimeTableEntry *timetable,
                                          uint n_sessions,
                                          TimeTableSpecifications *specs) {

  /* Avoid scheduling sessions with less strength to venues which have high
    capacity ex: don't schedule ECM-only class at auditorium */
  numeric violations = 0;

  for (uint i = 0; i < n_sessions; i++) {

    uint timeslot_id = timetable[i].timeslot;
    uint venue_id = timetable[i].venue;
    uint party_id_array[specs->party_table->size];
    uint number_of_parties;
    findAssociatedParties(i, &number_of_parties, party_id_array, specs);
    uint strength = 0;

    for (uint j = 0; j < number_of_parties;
         j++) { // Corrected the loop condition
      strength += specs->party_table->strength[party_id_array[j]];
    }
    uint capacity = specs->venue_table->capacity[venue_id];
    violations += capacity - strength;
  }

  return specs->constraint_weights[10] / violations;
}

numeric soft_constraint_common_timeslot_empty(TimeTableEntry *timetable,
                                              uint n_sessions,
                                              TimeTableSpecifications *specs) {

  /* Encourage common time slots being left empty for a particular batch,
  so if there are some sessions that were canceled during the week, they can be
  held at that time slot. */
  numeric violations = 0;

  uint timeslot_array[specs->timeslot_table->size];
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    timeslot_array[i] = 0;
  }

  for (uint i = 0; i < specs->session_table->size; i++) {
    uint timeslot_id = timetable[i].timeslot;
    uint venue_id = timetable[i].venue;
    timeslot_array[timeslot_id]++;
  }
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    if (timeslot_array[i] != 0) {
      violations++;
    }
  }

  return specs->constraint_weights[11] / violations;
}

numeric soft_constraint_minimize_backtoback_teacher_classes(
    TimeTableEntry *timetable, uint n_sessions,
    TimeTableSpecifications *specs) {

  /* Minimization of Back-to-Back Classes for teachers. */

  numeric violations = 0;
  uint number_of_days =
      specs->timeslot_table->day[specs->timeslot_table->size - 1];
  uint number_of_timeslot_per_day =
      specs->timeslot_table->size / number_of_days;

  for (uint i = 0; i < specs->party_table->size; i++) {

    if (specs->party_table->party_type[i] == 1) {

      uint session_id_array[specs->session_table->size];
      uint number_of_session;
      findAssociatedSessions(i, &number_of_session, session_id_array, specs);

      for (uint j = 0; j < number_of_session - 1; j = j + 2) {
        uint timeslot_id_prev = timetable[session_id_array[j]].timeslot;
        uint venue_id_prev = timetable[session_id_array[j]].venue;

        uint timeslot_id_next = timetable[session_id_array[j + 1]].timeslot;
        uint venue_id_next = timetable[session_id_array[j + 1]].venue;
        if ((timeslot_id_prev / number_of_timeslot_per_day) + 1 ==
            (timeslot_id_next / number_of_timeslot_per_day)) {
          violations++;
        }
      }
    }
  }

  return specs->constraint_weights[12] / violations;
}

numeric
soft_constraint_minimize_samecourse_sessions(TimeTableEntry *timetable,
                                             uint n_sessions,
                                             TimeTableSpecifications *specs) {

  /* Discourage scheduling sessions of the same course more than once per day.
   */
  numeric violations = 0;

  for (uint i = 0; i < specs->session_table->size; i++) {
    uint timeslot_id1 = timetable[i].timeslot;
    uint venue_id1 = timetable[i].venue;

    uint day1 = specs->timeslot_table->day[timeslot_id1];

    for (uint j = i + 1; j < specs->session_table->size; j++) {

      if (specs->session_table->course[i] == specs->session_table->course[j]) {
        uint timeslot_id2 = timetable[j].timeslot;
        uint venue_id2 = timetable[j].venue;

        uint day2 = specs->timeslot_table->day[timeslot_id2];
        if (day1 == day2) {
          violations++;
        }
      }
    }
  }

  return specs->constraint_weights[13] / violations;
}

numeric soft_constraint_evenly_throughout_week(TimeTableEntry *timetable,
                                               uint n_sessions,
                                               TimeTableSpecifications *specs) {
  /* Have an even distribution of events throughout the week (avoid having too
   * many sessions on one day). */
  numeric violations = 0;
  uint number_of_days =
      specs->timeslot_table->day[specs->timeslot_table->size - 1];

  for (uint i = 0; i < specs->party_table->size; i++) {
    uint session_id_array[specs->session_table->size];
    uint number_of_session;
    findAssociatedSessions(i, &number_of_session, session_id_array, specs);

    uint day_array[number_of_days];
    for (uint j = 0; j < number_of_days; j++) {
      day_array[j] = 0;
    }

    for (uint j = 0; j < number_of_session; j++) {

      uint timeslot_id = timetable[session_id_array[j]].timeslot;
      uint venue_id = timetable[session_id_array[j]].venue;
      uint day =
          specs->timeslot_table->day[timeslot_id]; // Corrected day access
      day_array[day]++;
    }

    uint sum = 0;
    for (uint j = 0; j < number_of_days; j++) {
      sum += day_array[j];
    }

    uint average = sum / number_of_days;
    for (uint j = 0; j < number_of_days; j++) {
      if (day_array[j] >= average) {
        violations += day_array[j] - average;
      } else {
        violations += average - day_array[j];
      }
    }
  }

  return specs->constraint_weights[15] * violations;
}

snumeric get_optimality(TimeTableEntry *timetable,
                        TimeTableSpecifications *specifications) {

  snumeric fitness = 0;

  fitness += soft_constraint_student_travel_time(
      timetable, specifications->session_table->size, specifications);

  fitness += soft_constraint_avoid_early_time(
      timetable, specifications->session_table->size, specifications);

  fitness += soft_constraint_common_timeslot_empty(
      timetable, specifications->session_table->size, specifications);

  fitness += soft_constraint_maximize_chunking(
      timetable, specifications->session_table->size, specifications);

  fitness += soft_constraint_minimize_backtoback_teacher_classes(
      timetable, specifications->session_table->size, specifications);

  fitness += soft_constraint_minimize_samecourse_sessions(
      timetable, specifications->session_table->size, specifications);

  fitness += soft_constraint_room_capacity_utilisation(
      timetable, specifications->session_table->size, specifications);

  fitness += soft_constraint_room_utlisation(
      timetable, specifications->session_table->size, specifications);

  fitness += soft_constraint_evenly_throughout_week(
      timetable, specifications->session_table->size, specifications);

  return fitness;
}
