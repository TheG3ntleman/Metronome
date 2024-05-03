from metronome.common.specifications import TimeTableSpecifications
from metronome.common.reduced_problem import Problem
from metronome.common.timetable import TimeTable

from typing import List

"""grammar_dictionary = {
  "<START_SPECIFICATIONS>": 0,
  "<END_SPECIFICATIONS>": 1,
  "<START_PROBLEM>": 2,
  "<END_PROBLEM>": 3,
  "<START_TIMETABLE>": 4,
  "<END_TIMETABLE>": 5,
  "<LOCALITY_TABLE>": 6,
  "<VENUE_TABLE>": 7,
  "<TIMESLOT_TABLE>": 8,
  "<PARTY_TABLE>": 9,
  "<SESSION_TABLE>": 10,
  "<ASSIGNMENT_TABLE>": 11,
  "<END_TABLE>": 12,
  "<SIZE>": 13,
  "<ID>": 14,
  "<DISTANCE>": 15,
  "<TYPE>": 16,
  "<CAPACITY>": 17,
  "<LOCALITY_ID>": 18,
  "<DAY>": 19,
  "<STRENGTH>": 20,
  "<MAX_HOUR>": 21,
  "<PREFERRED_START_TIME>": 22,
}"""




class StringGenerator:
  
  def __init__(self):
    # Initializing the symbol dictionary
    self.symbol_dictionary = {}
    
    self.symbols = [
      "<START_SPECIFICATIONS>",
      "<END_SPECIFICATIONS>",
      "<START_OPTIONS>",
      "<END_OPTIONS>",
      "<START_TIMETABLE>",
      "<END_TIMETABLE>",
      "<LOCALITY_TABLE>",
      "<VENUE_TABLE>",
      "<TIMESLOT_TABLE>",
      "<PARTY_TABLE>",
      "<SESSION_TABLE>",
      "<ASSIGNMENT_TABLE>",
      "<END_TABLE>",
      "<SIZE>",
      "<ID>",
      "<DISTANCE>",
      "<TYPE>",
      "<CAPACITY>",
      "<LOCALITY_ID>",
      "<DAY>",
      "<STRENGTH>",
      "<MAX_HOUR>",
      "<PREFERRED_START_TIME>",
      "<PREFERRED_END_TIME>",
      "<PREFERRED_BACK_TO_BACK>",
      "<PREFFERED_MAX_HOURS>",
      "<DURATION>",
      "<COURSE>",
      "<PARTY_ID>",
      "<SESSION_ID>",
      "<PRIORITY>",
      ",",
      "|",
      "~",
    ]

    for i, symbol in enumerate(self.symbols):
      self.symbol_dictionary[symbol] = i + 10 # Reserve 0 - 9 for actual digits.
      


  def generate_string(self, time_table_specifications : TimeTableSpecifications, problem : Problem, timetable : TimeTable) -> List[int]:
    string = []
    
    
     # Adding time table specifications to the string
    string.append(self.symbol_dictionary["<START_SPECIFICATIONS>"])
    
    # Adding timeslot table info
    string.append(self.symbol_dictionary["TIMESLOT_TABLE"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    #Insert the actual values here
    string.append(time_table_specifications.timeslot_table['size'])
    
    string.append(self.symbol_dictionary["<ID>"])
    # Insert the actual values here
    for timeslot_id in time_table_specifications.timeslot_table['size']:
      # Instead of appending timeslot_id as a whole we append the digits one by one.
      timeslot_id_str = str(timeslot_id)
      for digit in timeslot_id_str:
        string.append(int(digit))
      string.append(self.symbol_dictionary[","])
      
    
    string.append(self.symbol_dictionary["<DAY>"])
    #inserting the actual value 
    for day in time_table_specifications.timeslot_table['day']:
      day_str = str(day)
      for digit in day_str:
        string.append(int(digit))
      string.append(self.symbol_dictionary[","])
    
    # Ending the table
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Adding locality table info
    string.append(self.symbol_dictionary["LOCALITY_TABLE"])

    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual values here
    string.append(time_table_specifications.locality_table['size'])  

    string.append(self.symbol_dictionary["<ID>"])
    # Insert the actual values here 
    for locality_id in time_table_specifications.locality_table['ID']:
        locality_id_str = str(locality_id)
        for digit in locality_id_str:
            string.append(int(digit))
        string.append(self.symbol_dictionary[","])

    string.append(self.symbol_dictionary["<DISTANCE>"])
    # Insert the actual values here
    for distance in time_table_specifications.locality_table['Distance']:
        distance_str = str(distance)
        for digit in distance_str:
            string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<END_TABLE>"])

    
    # Adding venue table to the string
    string.append(self.symbol_dictionary["VENUE_TABLE"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual value for venue table size here
    string.append(time_table_specifications.venue_table['size'])
    
    string.append(self.symbol_dictionary["<ID>"])
    for venue_id in time_table_specifications.venue_table['id']:
        string.append(venue_id)
    
    string.append(self.symbol_dictionary["<TYPE>"])
    for venue_type in time_table_specifications.venue_table['type']:
        string.append(venue_type)
        
    string.append(self.symbol_dictionary["<CAPACITY>"])
    for capacity in time_table_specifications.venue_table['capacity']:
        string.append(capacity)
        
    string.append(self.symbol_dictionary["<LOCALITY_ID>"])
    for locality_id in time_table_specifications.venue_table['locality_id']:
        string.append(locality_id)
    
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Adding party table to the string
    string.append(self.symbol_dictionary["PARTY_TABLE"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual value for party table size here
    string.append(time_table_specifications.party_table['size'])
    
    string.append(self.symbol_dictionary["<ID>"])
    for party_id in time_table_specifications.party_table['id']:
        string.append(party_id)
    
    string.append(self.symbol_dictionary["<STRENGTH>"])
    for strength in time_table_specifications.party_table['strength']:
        string.append(strength)
        
    string.append(self.symbol_dictionary["<MAX_HOUR>"])
    for max_hour in time_table_specifications.party_table['max_hour']:
        string.append(max_hour)
        
    string.append(self.symbol_dictionary["<TYPE>"])
    for party_type in time_table_specifications.party_table['type']:
        string.append(party_type)
        
    string.append(self.symbol_dictionary["<PREFERRED_START_TIME>"])
    for start_time in time_table_specifications.party_table['preffered_start_time']:
        string.append(start_time)
    
    string.append(self.symbol_dictionary["<PREFERRED_END_TIME>"])
    for end_time in time_table_specifications.party_table['preffered_end_time']:
        string.append(end_time)
        
    string.append(self.symbol_dictionary["<PREFERRED_BACK_TO_BACK>"])
    for back_to_back in time_table_specifications.party_table['preffered_back_to_back']:
        string.append(back_to_back)
        
    string.append(self.symbol_dictionary["<PREFFERED_MAX_HOURS>"])
    for max_hours in time_table_specifications.party_table['preffered_max_hours']:
        string.append(max_hours)
    
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Adding session table to the string
    string.append(self.symbol_dictionary["SESSION_TABLE"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual value for session table size here
    string.append(time_table_specifications.sessions_table['size'])
    
    string.append(self.symbol_dictionary["<ID>"])
    for session_id in time_table_specifications.sessions_table['id']:
        string.append(session_id)
    
    string.append(self.symbol_dictionary["<TYPE>"])
    for session_type in time_table_specifications.sessions_table['type']:
        string.append(session_type)
        
    string.append(self.symbol_dictionary["<DURATION>"])
    for duration in time_table_specifications.sessions_table['duration']:
        string.append(duration)
        
    string.append(self.symbol_dictionary["<COURSE>"])
    for course in time_table_specifications.sessions_table['course']:
        string.append(course)
    
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Adding assignment table to the string
    string.append(self.symbol_dictionary["ASSIGNMENT_TABLE"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual value for assignment table size here
    string.append(time_table_specifications.assignment_table['Size'])
    
    string.append(self.symbol_dictionary["<PARTY_ID>"])
    for party_id in time_table_specifications.assignment_table['']:
        string.append(party_id)
    
    string.append(self.symbol_dictionary["<SESSION_ID>"])
    for session_id in time_table_specifications.assignment_table['Session ID']:
        string.append(session_id)
        
    string.append(self.symbol_dictionary["<PRIORITY>"])
    for priority in time_table_specifications.assignment_table['Priority']:
        string.append(priority)
    
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Ending the specifications
    string.append(self.symbol_dictionary["<END_SPECIFICATIONS>"])
  
  def generate_pretty_string(self, time_table_specifications : TimeTableSpecifications, problem : Problem, timetable : TimeTable) -> str:
    pass