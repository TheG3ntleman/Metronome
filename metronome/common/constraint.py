from specifications import TimeTableSpecifications
from metronome.timetable import TimeTable

class Constraints:
  
  def __init__(self, time_table_specifications : TimeTableSpecifications) -> None:
    self.time_table_specifications = time_table_specifications

  def hard_party_conflict(self, timetable: TimeTable, depth: int) -> int:
    number_of_violations = 0

    for party_id in range(self.time_table_specifications.number_of_parties):

      sessions = self.time_table_specifications.find_associated_sessions(party_id, depth)

      timeslot_counts = [0] * self.time_table_specifications.number_of_time_slots

      for session in sessions:
        if session > depth:
          continue
        timeslot_counts[timetable[session]["timeslot_id"]] += 1

      for count in timeslot_counts:
        if count > 1:
          number_of_violations += count - 1

    return number_of_violations
  

  def hard_repeated_tuple(self, timetable: TimeTable, depth: int) -> int:
    number_of_violations = 0

    tuple_counter = [[0] * self.time_table_specifications.number_of_venues for _ in range(self.time_table_specifications.number_of_time_slots)]

    for session_id in range(depth):
      tuple_counter[timetable[session_id]["timeslot_id"]][timetable[session_id]["venue_id"]] += 1

    for i in range(self.time_table_specifications.number_of_time_slots):
      for j in range(self.time_table_specifications.number_of_venues):
        if tuple_counter[i][j] > 1:
          number_of_violations += tuple_counter[i][j] - 1

    return number_of_violations


  def hard_venue_capacity(self, timetable: TimeTable, depth: int) -> int:
    number_of_violations = 0
    
    for session_id in range(depth):

      parties = self.time_table_specifications.find_associated_parties(session_id, depth)

      strength = 0

      for party in parties:
        strength += self.time_table_specifications.party_strengths[party]

      # Checking if the strength exceeds the venue capacity
      if strength > self.time_table_specifications.venue_capacities[timetable[session_id]["venue_id"]]:
        number_of_violations += 1

    return number_of_violations
  

  def hard_venue_type(self, timetable: TimeTable, depth: int) -> int:
    number_of_violations = 0
    
    for session_id in range(depth):
      if self.time_table_specifications.session_types[session_id] != self.time_table_specifications.venue_types[timetable[session_id]["venue_id"]]:
        number_of_violations += 1

    return number_of_violations
  

  def hard_max_hours(self, timetable: TimeTable, depth: int) -> int:
    number_of_violations = 0
    
    for party_id in range(self.time_table_specifications.number_of_parties):
      
      no_of_classes_per_day = [0] * 5  # Initialize the no_of_classes_per_day array to 0

      sessions = self.time_table_specifications.find_associated_sessions(party_id, depth)

      for session in sessions:
        timeslot_id = timetable[session]["timeslot_id"]
        day = self.time_table_specifications.time_slot_days[timeslot_id]
        no_of_classes_per_day[day] += 1

      # Checking if the number of sessions per day exceeds the maximum allowed hours
      for i in range(5):
        if no_of_classes_per_day[i] > self.time_table_specifications.party_max_hours[party_id]:
          number_of_violations += no_of_classes_per_day[i] - self.time_table_specifications.party_max_hours[party_id]

    return number_of_violations
  

  def hard_multi_timeslot(self, timetable: TimeTable, depth: int) -> int:
    number_of_violations = 0
    multi_time_slot_counter = 0
    bad_session = False

    for session_id in range(depth):
        
      if multi_time_slot_counter == 0:
        # Checking if the current session is a multi timeslot session
        # by checking if the duration is greater than 1.
        if self.time_table_specifications.session_durations[session_id] > 1:
            multi_time_slot_counter += self.time_table_specifications.session_durations[session_id]
      else:  # multi_time_slot_counter > 0
        # We check if the current timeslot is on the same day as the previous timeslot
        # if it is, then we check if the timeslot is right after the previous day's timeslot

        if bad_session:
          # If the previous sessions were not consecutive, then the
          # entire chain of sessions is treated as a sequence of violations from that 
          # point onwards.
          number_of_violations += 1
        else:
          previous_timeslot = timetable[session_id - 1]["timeslot_id"]
          current_timeslot = timetable[session_id]["timeslot_id"]

          if self.time_table_specifications.time_slot_days[previous_timeslot] != self.time_table_specifications.time_slot_days[current_timeslot]:
            number_of_violations += 1
            bad_session = True

          if self.time_table_specifications.time_slot_ids[previous_timeslot] + 1 != self.time_table_specifications.time_slot_ids[current_timeslot]:
            number_of_violations += 1
            bad_session = True

        multi_time_slot_counter -= 1

    return number_of_violations
  

  
  #@staticmethod
  
