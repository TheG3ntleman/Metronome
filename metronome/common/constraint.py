from .specifications import TimeTableSpecifications
import numpy as np
from metronome.common.timetable import TimeTable

class Constraints:
  
  def __init__(self, time_table_specifications : TimeTableSpecifications) -> None:
    self.time_table_specifications = time_table_specifications

  def hard_party_conflict(self, timetable : TimeTable, depth: int) -> int:
    
    number_of_violations = 0

    for party_id in range(self.time_table_specifications.party_table['size']):

      sessions = self.time_table_specifications.find_associated_sessions(party_id, depth)

      timeslot_counts = [0] * self.time_table_specifications.timeslot_table['size']

      for session in sessions:
        if session >= depth:
          continue
        timeslot_counts[timetable.timetable[session]["timeslot_id"]] += 1

      for count in timeslot_counts:
        if count > 1:
          number_of_violations += count - 1

    return number_of_violations
  

  def hard_repeated_tuple(self, timetable, depth: int) -> int:
    number_of_violations = 0

    tuple_counter = np.zeros((self.time_table_specifications.timeslot_table['size'], self.time_table_specifications.venue_table['size']))
    for session_id in range(depth):
      tuple_counter[timetable.timetable[session_id]["timeslot_id"]][timetable.timetable[session_id]["venue_id"]] += 1
  
    for i in range(self.time_table_specifications.timeslot_table['size']):
      for j in range(self.time_table_specifications.venue_table['size']):
        if tuple_counter[i][j] > 1:
          number_of_violations += tuple_counter[i][j] - 1

    #print(tuple_counter)
    #print(number_of_violations)
    #exit()

    return number_of_violations


  def hard_venue_capacity(self, timetable, depth: int) -> int:
    number_of_violations = 0
    
    for session_id in range(depth):

      parties = self.time_table_specifications.find_associated_parties(session_id, depth)

      strength = 0

      for party in parties:
        strength += self.time_table_specifications.party_table['strength'][party]

      # Checking if the strength exceeds the venue capacity
      number_of_violations += max(0, strength - self.time_table_specifications.venue_table['capacity'][timetable[session_id]['venue_id']])#venue_capacities[timetable.timetable[session_id]["venue_id"]])

    return number_of_violations
  

  def hard_venue_type(self, timetable, depth: int) -> int:
    number_of_violations = 0
    
    for session_id in range(depth):
      if self.time_table_specifications.sessions_table['type'][session_id]!= self.time_table_specifications.venue_table['type'][timetable[session_id]['venue_id']]:#venue_types[timetable.timetable[session_id]["venue_id"]]:
        number_of_violations += 1

    return number_of_violations
  

  def hard_max_hours(self, timetable, depth: int) -> int:
    number_of_violations = 0
    
    for party_id in range(self.time_table_specifications.party_table['size']):
      
      if self.time_table_specifications.party_table['type'][party_id] == 0:
        continue

      no_of_classes_per_day = [0] * 5  # Initialize the no_of_classes_per_day array to 0

      sessions = self.time_table_specifications.find_associated_sessions(party_id, depth)

      for session in sessions:
        if session > depth:
          continue
        timeslot_id = timetable.timetable[session]["timeslot_id"]
        day = self.time_table_specifications.timeslot_table['day'][timeslot_id]#time_slot_days[timeslot_id]
        no_of_classes_per_day[day] += 1

      # Checking if the number of sessions per day exceeds the maximum allowed hours
      for i in range(5):
        if no_of_classes_per_day[i] > self.time_table_specifications.party_table['max_hours'][party_id]:#party_max_hours[party_id]:
          number_of_violations += no_of_classes_per_day[i] - self.time_table_specifications.party_table['max_hours'][party_id]#party_max_hours[party_id]

    return number_of_violations
  

  def hard_multi_timeslot(self, timetable, depth: int) -> int:
   
    number_of_violations = 0
    '''
    multi_time_slot_counter = 0
    bad_session = False

    for session_id in range(depth):
        
      if multi_time_slot_counter == 0:

        if self.time_table_specifications.session_durations[session_id] > 1:
            multi_time_slot_counter += self.time_table_specifications.session_durations[session_id]
      else:  # multi_time_slot_counter > 0

        if bad_session:
          number_of_violations += 1
        else:
          previous_timeslot = timetable.timetable[session_id - 1]["timeslot_id"]
          current_timeslot = timetable.timetable[session_id]["timeslot_id"]

          if self.time_table_specifications.time_slot_days[previous_timeslot] != self.time_table_specifications.time_slot_days[current_timeslot]:
            number_of_violations += 1
            bad_session = True

          if self.time_table_specifications.time_slot_ids[previous_timeslot] + 1 != self.time_table_specifications.time_slot_ids[current_timeslot]:
            number_of_violations += 1
            bad_session = True

        multi_time_slot_counter -= 1
    '''
    session_id = 0
    while session_id < depth:

      if self.time_table_specifications.sessions_table['duration'][session_id] > 1:#session_durations[session_id] > 1:
        duration = self.time_table_specifications.sessions_table['duration'][session_id]#session_durations[session_id]

        for j in range(session_id + 1, session_id + duration):
          
          if timetable.timetable[session_id]["timeslot_id"] + j - session_id != timetable.timetable[j]["timeslot_id"]:
            number_of_violations += 1
          else:
            if timetable.timetable[session_id]["venue_id"] != timetable.timetable[j]["venue_id"]:
              number_of_violations += 1

        session_id += duration
      session_id += 1  

    return number_of_violations
  

  def soft_travel_time(self, timetable, depth: int) -> int:
    aggregate_travel_time = 0

    for party_id in range(self.time_table_specifications.party_table['size']):#.number_of_parties):
      # Fetching the associated sessions
      
      sessions = self.time_table_specifications.find_associated_sessions(party_id, depth)

      # Iterating through the sessions and adding the values to travel time
      for i in range(len(sessions)):
        # There is no need to add travel time in chronological order
        # as the travel time is symmetric

        # We find the session on the same day and the immediately next timeslot
        # and add the distance between their venue locality's to the 
        # aggregate travel time.
        current_timeslot_id = timetable.timetable[sessions[i]]["timeslot_id"]
        current_timeslot_day = self.time_table_specifications.timeslot_table['day'][current_timeslot_id]#time_slot_days[current_timeslot_id]

        # We check if there is another session immediately after the current session
        # and if it is on the same day
        for j in range(i + 1, len(sessions)):
          next_timeslot_id = timetable.timetable[sessions[j]]["timeslot_id"]
          next_timeslot_day = self.time_table_specifications.timeslot_table['day'][next_timeslot_id]#time_slot_days[next_timeslot_id]

          if (current_timeslot_day == next_timeslot_day and
                  self.time_table_specifications.timeslot_table['id'][current_timeslot_id] + 1 == #time_slot_ids[current_timeslot_id] + 1 ==
                  self.time_table_specifications.timeslot_table['id'][next_timeslot_id]):#time_slot_ids[next_timeslot_id]):
            first_venue_id = timetable.timetable[sessions[i]]["venue_id"]
            second_venue_id = timetable.timetable[sessions[j]]["venue_id"]
            if first_venue_id != second_venue_id:
              # Find corresponding locality
              first_locality_id = self.time_table_specifications.venue_table['locality'][first_venue_id]#venue_locality[first_venue_id]
              second_locality_id = self.time_table_specifications.venue_table['locality'][second_venue_id]#venue_locality[second_venue_id]
              x = first_locality_id
              y = second_locality_id
              n = self.time_table_specifications.locality_table['size']#number_of_localities
              result = int(x*n - (x*(x-1)/2) + (y-x+1))
              distance = self.time_table_specifications.locality_table['distance'][result - 1]#locality_distances[result - 1]
              aggregate_travel_time += distance

    return aggregate_travel_time
  

  def soft_chunking(self, timetable, depth: int) -> int:
    aggregate_number_of_gaps = 0

    for party_id in range(self.time_table_specifications.party_table['size']):#.number_of_parties):
        
      sessions = self.time_table_specifications.find_associated_sessions(party_id, depth)
      
      # Sort sessions by timeslot id
      sessions.sort(key=lambda x: timetable.timetable[x]["timeslot_id"])

      # Now we iterate through the sorted sessions and check if there are any gaps
      for i in range(len(sessions) - 1):
        # Checking if the current session and the next are on the same day
        current_timeslot_id = timetable.timetable[sessions[i]]["timeslot_id"]
        next_timeslot_id = timetable.timetable[sessions[i + 1]]["timeslot_id"]

        # Checking if they are on the same day
        #if self.time_table_specifications.time_slot_days[current_timeslot_id] == self.time_table_specifications.time_slot_days[next_timeslot_id]:
        if self.time_table_specifications.timeslot_table['day'][current_timeslot_id] == self.time_table_specifications.timeslot_table['day'][next_timeslot_id]: 
          #aggregate_number_of_gaps += self.time_table_specifications.time_slot_ids[next_timeslot_id] - (self.time_table_specifications.time_slot_ids[current_timeslot_id] - 1)
          aggregate_number_of_gaps += self.time_table_specifications.timeslot_table['id'][next_timeslot_id] - (self.time_table_specifications.timeslot_table['id'][current_timeslot_id] - 1)

    return aggregate_number_of_gaps
  

  def soft_room_utilization(self, timetable, depth: int) -> int:
    number_of_underused_venues = 0
    venue_usage = [0] * self.time_table_specifications.venue_table['size']#number_of_venues

    for session_id in range(self.time_table_specifications.sessions_table['size']):#number_of_sessions):
      venue_usage[timetable.timetable[session_id]["venue_id"]] = 1

    # Calculating the mean and standard deviation of the venue usage
    mean = sum(venue_usage) / len(venue_usage)
    standard_deviation = (sum((x - mean) ** 2 for x in venue_usage) / len(venue_usage)) ** 0.5

    # If the usage is 2 stddev below the mean, add to the number of underused venues
    for usage in venue_usage:
      if usage < mean - 2 * standard_deviation:
        number_of_underused_venues += 1

    return number_of_underused_venues
  

  def soft_extreme_time(self, timetable, depth: int) -> int:
    aggregate_extreme_time = 0

    # We iterate through sessions
    for session_id in range(self.time_table_specifications.sessions_table['size']):#number_of_sessions):
      # We find the associated parties
      
      parties = self.time_table_specifications.find_associated_parties(session_id, depth)

      # We find the day and id of the session
      current_timeslot_id = timetable.timetable[session_id]["timeslot_id"] % 7  # 7 is the number of timeslots per day

      # Iterating through the appropriate parties and checking if this does not lie in 
      # preferred start time and preferred end time
      for party_id in parties:
        if current_timeslot_id < self.time_table_specifications.party_table['preffered_start_time'][party_id] or \
          current_timeslot_id > self.time_table_specifications.party_table['preffered_end_time'][party_id]:
          aggregate_extreme_time += 1

    return aggregate_extreme_time
  

  def soft_room_capacity_utilization(self, timetable, depth: int) -> int:
    aggregate_capacity_gaps = 0

    for session_id in range(self.time_table_specifications.sessions_table['size']):#number_of_sessions):
        
      parties = self.time_table_specifications.find_associated_parties(session_id, depth)
      
      combined_strength = sum(self.time_table_specifications.party_table['strength'][party_id] for party_id in parties)#party_strengths[party_id] for party_id in parties)

      # We do not want this to work against the hard constraint
      if combined_strength < self.time_table_specifications.venue_table['capacity'][timetable[session_id]['venue_id']]:#venue_capacities[timetable.timetable[session_id]["venue_id"]]:
        aggregate_capacity_gaps += self.time_table_specifications.venue_table['capacity'][timetable[session_id]['venue_id']] - combined_strength#venue_capacities[timetable.timetable[session_id]["venue_id"]] - combined_strength
      
    return aggregate_capacity_gaps/(depth+1)
  

  def soft_common_timeslot_empty(self, timetable, depth: int) -> int:
    # Placeholder function
    # A more detailed discussion and implementation are needed
    return 0


  def soft_minimize_back_to_back(self, timetable, depth: int) -> int:
    aggregate_back_to_back = 0

    # Iterating through parties
    for party_id in range(self.time_table_specifications.party_table.size):#number_of_parties):
      # Checking if the party is a professor
      if self.time_table_specifications.party_table['id'][party_id] != 1:#party_type[party_id] != 1:
        continue

      sessions = self.time_table_specifications.find_associated_sessions(party_id, depth)
      
      # We sort the sessions by timeslot id
      sessions.sort(key=lambda x: timetable.timetable[x]["timeslot_id"])

      # Now we iterate through the sorted sessions and check if there are any back to back
      for i in range(len(sessions) - 1):
        current_timeslot_id = timetable.timetable[sessions[i]]["timeslot_id"]
        next_timeslot_id = timetable.timetable[sessions[i + 1]]["timeslot_id"]

        # Checking if they are on the same day
        #if self.time_table_specifications.time_slot_days[current_timeslot_id] == self.time_table_specifications.time_slot_days[next_timeslot_id]:
        if self.time_table_specifications.timeslot_table['day'][current_timeslot_id] == self.time_table_specifications.timeslot_table['day'][next_timeslot_id]:
          #if self.time_table_specifications.time_slot_ids[next_timeslot_id] == self.time_table_specifications.time_slot_ids[current_timeslot_id] + 1:
          if self.time_table_specifications.timeslot_table['id'][next_timeslot_id] == self.time_table_specifications.timeslot_table['id'][current_timeslot_id] + 1:
            aggregate_back_to_back += 1

    return aggregate_back_to_back
  

  def soft_repeated_course_session(self, timetable, depth: int) -> int:
    # Placeholder function
    # Requires a course table for efficient implementation
    return 0
  

  def soft_sessions_well_distributed(self, timetable, depth: int) -> int:
    aggregate_less_used_timeslots = 0
    timeslot_usage = [0] * self.time_table_specifications.timeslot_table['size']#number_of_time_slots

    # Iterating through sessions and counting the number of sessions in each timeslot
    for i in range(depth):
      timeslot_usage[timetable.timetable[i]["timeslot_id"]] += 1

    # Calculating the mean and standard deviation of the timeslot usage
    mean = sum(timeslot_usage) / len(timeslot_usage)
    standard_deviation = sum((count - mean) ** 2 for count in timeslot_usage) / len(timeslot_usage)

    # If the usage is 2 stddev below the mean, add to the number of underused venues
    for count in timeslot_usage:
      if count < mean - 2 * standard_deviation:
        aggregate_less_used_timeslots += 1

    return aggregate_less_used_timeslots
  

  def soft_lab_after_lecture(self, timetable, depth: int) -> int:
    # Placeholder function awaiting further implementation
    aggregate_lab_after_lecture = 0
    # An efficient implementation would require access to a venue type table
    return aggregate_lab_after_lecture
  
  def is_feasible(self, timetable, depth):
    # Check if the assignment satisfies all hard constraints
    if (self.hard_party_conflict(timetable, depth) == 0 and
        self.hard_repeated_tuple(timetable, depth) == 0 and
        self.hard_venue_capacity(timetable, depth) == 0 and
        self.hard_venue_type(timetable, depth) == 0 and
        self.hard_max_hours(timetable, depth) == 0 and
        self.hard_multi_timeslot(timetable, depth) == 0):
      return True
    return False
  
  def evaluate_soft_constraints(self, timetable, depth):
    # Calculate the total number of violations for the session assignment based on soft constraints
    violations = (
      self.soft_travel_time(timetable, depth) +
      self.soft_chunking(timetable, depth) +
      self.soft_room_utilization(timetable, depth) +
      self.soft_extreme_time(timetable, depth) +
      self.soft_room_capacity_utilization(timetable, depth) +
      self.soft_common_timeslot_empty(timetable, depth) +
      self.soft_minimize_back_to_back(timetable, depth) +
      self.soft_repeated_course_session(timetable, depth) +
      self.soft_sessions_well_distributed(timetable, depth) +
      self.soft_lab_after_lecture(timetable, depth)
    )
    return violations
  #@staticmethod
  