#include "constraint.h"
#include "src/timetable/population.h"
#include "specifications.h"
#include "specifications.h"
#include <linux/limits.h>
#include <sys/types.h>

uint hard_party_conflict(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs,
                                    uint depth) {

  uint number_of_violations = 0;

  // Iterating through the parties
  for (uint party_id = 0; party_id < specs->party_table->size; party_id++) {
    uint number_of_sessions = 0;
    uint sessions[specs->session_table->size];
    findAssociatedSessions(party_id, &number_of_sessions, sessions, depth,
                           specs);

    uint timeslot_counts[specs->timeslot_table->size];

    // Initialize the timeslot_counts array to 0
    for (uint i = 0; i < specs->timeslot_table->size; i++)
      timeslot_counts[i] = 0;

    for (uint i = 0; i < number_of_sessions; i++) {
      // To make sure a party is not in two different placese at the
      // same time. We iterate through all the tuples corresponding to
      // the parties associated sessions and see if at any point, the
      // timeslot value of the tuples are the same. This is done
      // by having a counter for each timeslot.

      if (sessions[i] > depth)
        continue;

      timeslot_counts[timetable[sessions[i]].timeslot]++;
    }

    // Accumulate violations here
    for (uint i = 0; i < specs->timeslot_table->size; i++) {
      if (timeslot_counts[i] > 1) {
        number_of_violations += timeslot_counts[i] - 1;
      }
    }
  }

  return number_of_violations;
}

uint hard_repeated_tuple(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs,
                                    uint depth) {

  uint number_of_violations = 0;

  uint tuple_counter[specs->timeslot_table->size][specs->venue_table->size];

  // Initialize the tuple_counter array to 0
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    for (uint j = 0; j < specs->venue_table->size; j++) {
      tuple_counter[i][j] = 0;
    }
  }

  // Iterate through all sessions
  for (uint i = 0; i < depth; i++) {
    tuple_counter[timetable[i].timeslot][timetable[i].venue] += 1;
  }

  // Now we accumulate the violations
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    for (uint j = 0; j < specs->venue_table->size; j++) {
      if (tuple_counter[i][j] > 1) {
        number_of_violations += tuple_counter[i][j] - 1;
      }
    }
  }

  return number_of_violations;
}

uint hard_venue_capacity(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs,
                                    uint depth) {

  uint number_of_violations = 0;
  for (uint session_id = 0; session_id < depth; session_id++) {
    uint number_of_parties = 0;
    uint party_id[specs->party_table->size];
    findAssociatedParties(session_id, &number_of_parties, party_id, depth,
                          specs);

    uint strength = 0;

    for (uint i = 0; i < number_of_parties; i++) {
      strength += specs->party_table->strength[party_id[i]];
    }

    // Checking if the strength exceeds the venue capacity
    if (strength > specs->venue_table->capacity[timetable[session_id].venue]) {
      number_of_violations += 1;
    }
  }

  return number_of_violations;
}

uint hard_venue_type(TimeTableTuple *timetable,
                                TimeTableSpecifications *specs, uint depth) {
  uint number_of_violations = 0;
  for (uint session_id = 0; session_id < depth; session_id++) {
    if (specs->session_table->type[session_id] !=
        specs->venue_table->type[timetable[session_id].venue]) {
      number_of_violations += 1;
    }
  }

  return number_of_violations;
}

uint hard_max_hours(TimeTableTuple *timetable,
                               TimeTableSpecifications *specs, uint depth) {

  uint number_of_violations = 0;

  for (uint party_id = 0; party_id < specs->party_table->size; party_id++) {
    uint number_of_sessions = 0;
    uint sessions[specs->session_table->size];
    uint no_of_classes_per_day[5]; // BIG ASSUMPTION CHANGE LATER

    // Initialize the no_of_classes_per_day array to 0
    for (uint i = 0; i < 5; i++)
      no_of_classes_per_day[i] = 0;

    findAssociatedSessions(party_id, &number_of_sessions, sessions, depth,
                           specs);

    for (uint i = 0; i < number_of_sessions; i++) {
      uint timeslot_id = timetable[sessions[i]].timeslot;
      uint day = specs->timeslot_table->day[timeslot_id];
      no_of_classes_per_day[day]++;
    }

    // Checking if the no of sessions per day are exceeded by the threshold
    // amount
    for (uint i = 0; i < 5; i++) {
      if (no_of_classes_per_day[i] > specs->party_table->max_hours[party_id]) {
        number_of_violations +=
            no_of_classes_per_day[i] - specs->party_table->max_hours[party_id];
      }
    }
  }

  return number_of_violations;
}

uint hard_multi_timeslot(TimeTableTuple *timetable,
                                    TimeTableSpecifications *specs,
                                    uint depth) {

  uint number_of_violations = 0;
  uint multi_time_slot_counter = 0;
  char bad_session = 0;

  for (uint session_id = 0; session_id < depth; session_id++) {
    
    if (multi_time_slot_counter == 0) {
      // Checking if the current session is a multi timeslot session
      // by checking if the duration is greater than 1.
       if (specs->session_table->duration[session_id] > 1) {
        multi_time_slot_counter += specs->session_table->duration[session_id];
       }
    } else { // multi_time_slot_counter > 0
      // We check if the current timeslot is on the same day as the previous timeslot
      // if it is, then we check if the timeslot is right after the previous day's timeslot

      if (bad_session) 
        // If the previous sessions were not consecutive, then the
        // entire chain of sessions is treated as a sequence of violations from that 
        // point onwards.
        number_of_violations++;
      else {

        uint previous_timeslot = timetable[session_id - 1].timeslot;
        uint current_timeslot = timetable[session_id].timeslot;

        if (specs->timeslot_table->day[previous_timeslot] !=
            specs->timeslot_table->day[current_timeslot]) {
          number_of_violations += 1;
          bad_session = 1;
        }

        if (specs->timeslot_table->id[previous_timeslot] + 1 !=
            specs->timeslot_table->id[current_timeslot]) {
          number_of_violations += 1;
          bad_session = 1;
        }
      }

      multi_time_slot_counter -= 1;

    }

  }

  return number_of_violations;
}

uint soft_travel_time(TimeTableTuple *timetable,
                          TimeTableSpecifications *specs, uint depth) {

  uint aggregate_travel_time = 0;

  for (uint party_id = 0; party_id < specs->party_table->size; party_id++) {
    // Fetching the associated sessions
    uint number_of_sessions;
    uint sessions[specs->session_table->size];
    findAssociatedSessions(party_id, &number_of_sessions, sessions, depth,
                           specs);

    // Iterating through the sessions and adding the values to travel time
    for (uint i = 0; i < number_of_sessions; i++) {
      // There is no need to add travel time in chronological order
      // as the travel time is symmetric

      // We find the session on the same day and the immeditately next timeslot
      // and add the distance between their venue locality's to the 
      // aggregate travel time.
      uint current_timeslot_id = timetable[sessions[i]].timeslot;
      uint current_timeslot_day = specs->timeslot_table->day[current_timeslot_id];

      // We check if there is another session immediately after the current session
      // and if it is on the same day
      for (uint j = i + 1; j < number_of_sessions; j++) {
        uint next_timeslot_id = timetable[sessions[j]].timeslot;
        uint next_timeslot_day = specs->timeslot_table->day[next_timeslot_id];

        if (current_timeslot_day == next_timeslot_day &&
            specs->timeslot_table->id[current_timeslot_id] + 1 ==
                specs->timeslot_table->id[next_timeslot_id]) {
            uint first_venue_id = timetable[sessions[i]].venue;
            uint second_venue_id = timetable[sessions[j]].venue;
            if (first_venue_id != second_venue_id) {
              // Find corresponding locality
              uint first_locality_id = specs->venue_table->locality[first_venue_id];
              uint second_locality_id = specs->venue_table->locality[second_venue_id];
              uint distance = 0;
              locality_to_distance(first_locality_id, second_locality_id, &distance,  specs);
              aggregate_travel_time += distance;
            } 
        }
      }

    }
  }

  return aggregate_travel_time;

}

uint soft_chunking(TimeTableTuple *timetable, TimeTableSpecifications *specs,
                   uint depth) {

  return 0;

  uint aggregate_number_of_gaps = 0;

  for (uint party_id = 0; party_id < specs->party_table->size; party_id++) {
    uint number_of_sessions;
    uint sessions[specs->session_table->size];
    findAssociatedSessions(party_id, &number_of_sessions, sessions, depth,
                           specs);
    
    // Sort sessions by timeslot id
    for (uint i = 0; i < number_of_sessions; i++) {
      for (uint j = i + 1; j < number_of_sessions; j++) {
        if (timetable[sessions[i]].timeslot > timetable[sessions[j]].timeslot) {
          uint temp = sessions[i];
          sessions[i] = sessions[j];
          sessions[j] = temp;
        }
      }
    }

    // Now we iterate through the sorted sessions and check if there are any gaps
    for (uint i = 0; i < number_of_sessions - 1; i++) {
      // Checking if the current session and the next are on the same day
      
      uint current_timeslot_id = timetable[sessions[i]].timeslot;
      uint next_timeslot_id = timetable[sessions[i + 1]].timeslot;

      // Checking if they are on the same day
      if (specs->timeslot_table->day[current_timeslot_id] ==
          specs->timeslot_table->day[next_timeslot_id]) {
        aggregate_number_of_gaps += specs->timeslot_table->id[next_timeslot_id] -
                         (specs->timeslot_table->id[current_timeslot_id] - 1);
      }

    }

  }

  return aggregate_number_of_gaps;
}

uint soft_room_utilization(TimeTableTuple *timetable,
                           TimeTableSpecifications *specs, uint depth) {

  uint number_of_underused_venues = 0;
  uint venue_usage[specs->venue_table->size];

  for (uint session_id = 0; session_id < specs->session_table->size; session_id++) {
    venue_usage[timetable[session_id].venue] = 1;
  }

  // Calculating the mean and standard deviation of the venue usage
  snumeric mean = 0;
  for (uint i = 0; i < specs->venue_table->size; i++) {
    mean += venue_usage[i];
  }
  mean /= specs->venue_table->size;

  snumeric standard_deviation = 0;
  for (uint i = 0; i < specs->venue_table->size; i++) {
    standard_deviation += (venue_usage[i] - mean) * (venue_usage[i] - mean);
  }
  standard_deviation /= specs->venue_table->size;

  // If the usage is 2 stddev below the mean  add to the number of underused venues
  for (uint i = 0; i < specs->venue_table->size; i++) {
    if (venue_usage[i] < mean - 2 * standard_deviation) {
      number_of_underused_venues++;
    }
  }

  return number_of_underused_venues;

}

uint soft_extreme_time(TimeTableTuple *timetable,
                       TimeTableSpecifications *specs, uint depth) {

  uint aggregate_extreme_time = 0;

  // We iterate through sessions
  for (uint session_id = 0; session_id < specs->session_table->size; session_id++) {
    // We find the associated parties
    uint number_of_parties;
    uint parties[specs->party_table->size];
    findAssociatedParties(session_id, &number_of_parties, parties, depth,
                          specs);

    // We find the day and id of the session
    uint current_timeslot_id = timetable[session_id].timeslot % 7; // 7 is the number of timeslots per day

    // Iterating through the appropriate parties and checking if this does not lie in 
    // preffered start time and preferred end time
    for (uint i = 0; i < number_of_parties; i++) {
      if (current_timeslot_id < specs->party_table->preferred_start_time[parties[i]] ||
          current_timeslot_id > specs->party_table->preferred_end_time[parties[i]]) {
        aggregate_extreme_time += 1;
      }
    }
  }

  return aggregate_extreme_time;

}

uint soft_room_capacity_utilization(TimeTableTuple *timetable,
                           TimeTableSpecifications *specs, uint depth) {


  uint aggregate_capacity_gaps = 0;

  for (uint session_id = 0; session_id < specs->session_table->size; session_id++) {
    uint number_of_parties;
    uint parties[specs->party_table->size];
    findAssociatedParties(session_id, &number_of_parties, parties, depth,
                          specs);
    
    uint combined_strength = 0;
    for (uint i = 0; i < number_of_parties; i++) {
      combined_strength += specs->party_table->strength[parties[i]];
    }

    // We do not want this to work against the hard constraint so
    if (combined_strength < specs->venue_table->capacity[timetable[session_id].venue]) {
      aggregate_capacity_gaps += specs->venue_table->capacity[timetable[session_id].venue] - combined_strength;
    } // otherwise do not make a fuss.
    
  }

  return aggregate_capacity_gaps;

}

uint soft_common_timeslot_empty(TimeTableTuple *timetable,
                                TimeTableSpecifications *specs, uint depth) {

  // We would like a common timeslot to be empty for all parties, this will add more
  // flexibility to the timetable

  return 0; // A more detailed discussion is needed to implement this

}

uint soft_minimize_back_to_back(
    TimeTableTuple *timetable, TimeTableSpecifications *specs,
    uint depth){ // Only for professors, sorry students :(

  /*uint aggregate_back_to_back = 0;

  // Iterating through parties
  for (uint party_id = 0; party_id < specs->party_table->size; party_id++) {
    // Checking if the party is a professor
    if (specs->party_table->type[party_id] != 1)
      continue;

    uint number_of_sessions;
    uint sessions[specs->session_table->size];
    findAssociatedSessions(party_id, &number_of_sessions, sessions, depth,
                           specs);

    // We sort the sessions by timeslot id
    for (uint i = 0; i < number_of_sessions; i++) {
      for (uint j = i + 1; j < number_of_sessions; j++) {
        if (timetable[sessions[i]].timeslot > timetable[sessions[j]].timeslot) {
          uint temp = sessions[i];
          sessions[i] = sessions[j];
          sessions[j] = temp;
        }
      }
    }

    // Now we iterate through the sorted sessions and check if there are any back to back
    for (uint i = 0; i < number_of_sessions - 1; i++) {
      // Checking if the current session and the next are on the same day
      
      uint current_timeslot_id = timetable[sessions[i]].timeslot;
      uint next_timeslot_id = timetable[sessions[i + 1]].timeslot;

      // Checking if they are on the same day
      if (specs->timeslot_table->day[current_timeslot_id] ==
          specs->timeslot_table->day[next_timeslot_id]) {
        if (specs->timeslot_table->id[next_timeslot_id] ==
            specs->timeslot_table->id[current_timeslot_id] + 1) {
          aggregate_back_to_back += 1;
        }
      }

    }

  }*/

  // Party type is not implemented yet that is why I skipped this for now.

  return 0;

}

uint soft_repeated_course_session(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth) {

  // A course table is needed to implement this effeciently. 

}

uint soft_sessions_well_distributed(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth) {

  // I don't like this, I want all classes to be at once in the week and then 
  // have nothing at all. PLEASE DISABLE THIS.

  uint aggregate_less_used_timeslots = 0;

  uint timeslot_usage[specs->timeslot_table->size];

  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    timeslot_usage[i] = 0;
  }

  // Iterating through sessions and counting the number of sessions in each timeslot
  for (uint i = 0; i < depth; i++) {
    timeslot_usage[timetable[i].timeslot]++;
  }

  // Calculating the mean and standard deviation of the timeslot usage
  snumeric mean = 0;
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    mean += timeslot_usage[i];
  }
  mean /= specs->timeslot_table->size;

  snumeric standard_deviation = 0;
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    standard_deviation += (timeslot_usage[i] - mean) * (timeslot_usage[i] - mean);
  }
  standard_deviation /= specs->timeslot_table->size;

  // If the usage is 2 stddev below the mean  add to the number of underused venues
  for (uint i = 0; i < specs->timeslot_table->size; i++) {
    if (timeslot_usage[i] < mean - 2 * standard_deviation) {
      aggregate_less_used_timeslots++;
    }
  }

  return aggregate_less_used_timeslots;

}

uint soft_lab_after_lecture(TimeTableTuple *timetable,
                         TimeTableSpecifications *specs, uint depth) {

  uint aggregate_lab_after_lecture = 0;

  // An effecient implementation would require a venue type table

  return aggregate_lab_after_lecture;

}