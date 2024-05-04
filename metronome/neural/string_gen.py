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
      
    # Printing key-value pairs of the symbol_dictionary
    print("the symbol_dictionary with its key value pair is: \n")
    for symbol, value in self.symbol_dictionary.items():
        print(f"Symbol: {symbol}, Value: {value}")


  def generate_string(self, time_table_specifications : TimeTableSpecifications, problem : Problem, timetable : TimeTable, depth : int) -> List[int]:
    string = []
    
    
     # Adding time table specifications to the string
    string.append(self.symbol_dictionary["<START_SPECIFICATIONS>"])
    
    # Adding timeslot table info
    string.append(self.symbol_dictionary["<TIMESLOT_TABLE>"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    #Insert the actual values here
    string.append(time_table_specifications.timeslot_table['size'])
    
    string.append(self.symbol_dictionary["<ID>"])
    # Insert the actual values here
    for timeslot_id in range(time_table_specifications.timeslot_table['size']):
      # Instead of appending timeslot_id as a whole we append the digits one by one.
      timeslot_id_str = str(timeslot_id)
      for digit in timeslot_id_str:
        string.append(int(digit))
      string.append(self.symbol_dictionary[","])
      
    string.append(self.symbol_dictionary["<DAY>"])
    # insert the actual value 
    for day in time_table_specifications.timeslot_table['day']:
      day_str = str(day)
      for digit in day_str:
        string.append(int(digit))
      string.append(self.symbol_dictionary[","])
    
    # Ending the table
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Adding locality table info
    string.append(self.symbol_dictionary["<LOCALITY_TABLE>"])

    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual values here
    string.append(time_table_specifications.locality_table['size'])  

    string.append(self.symbol_dictionary["<ID>"])
    # Insert the actual values here 
    for locality_id in time_table_specifications.locality_table['id']:
        locality_id_str = str(locality_id)
        for digit in locality_id_str:
            string.append(int(digit))
        string.append(self.symbol_dictionary[","])

    string.append(self.symbol_dictionary["<DISTANCE>"])
    # Insert the actual values here
    for distance in time_table_specifications.locality_table['distance']:
        distance_str = str(distance)
        for digit in distance_str:
            string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<END_TABLE>"])

    
    # Adding venue table to the string
    string.append(self.symbol_dictionary["<VENUE_TABLE>"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual value for venue table size here
    string.append(time_table_specifications.venue_table['size'])
    
    string.append(self.symbol_dictionary["<ID>"])
    # insert the actual values here
    for venue_id in time_table_specifications.venue_table['id']:
        venue_id_str = str(venue_id)
        for digit in venue_id_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<TYPE>"])
    # insert the actual value here
    for venue_type in time_table_specifications.venue_table['type']:
        venue_type_str = str(venue_type)
        for digit in venue_type_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])   
        
    string.append(self.symbol_dictionary["<CAPACITY>"])
    # insert the actual value here
    for capacity in time_table_specifications.venue_table['capacity']:
        capacity_str = str(capacity)
        for digit in capacity_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<LOCALITY_ID>"])
    # insert the actual value here
    for locality_id in time_table_specifications.venue_table['locality_id']:
        locality_id_str = str(locality_id)
        for digit in locality_id_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Adding party table to the string
    string.append(self.symbol_dictionary["<PARTY_TABLE>"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual value for party table size here
    string.append(time_table_specifications.party_table['size'])
    
    string.append(self.symbol_dictionary["<ID>"])
    # insert the actual value here
    for party_id in time_table_specifications.party_table['id']:
        party_id_str = str(party_id)
        for digit in party_id_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<STRENGTH>"])
    # insert the actual value here
    for strength in time_table_specifications.party_table['strength']:
        strength_str = str(strength)
        for digit in strength_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<MAX_HOUR>"])
    # insert the actual value here
    for max_hour in time_table_specifications.party_table['max_hour']:
        max_hour_str = str(max_hour)
        for digit in max_hour_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])

    string.append(self.symbol_dictionary["<TYPE>"])
    # insert the actual value here
    for party_type in time_table_specifications.party_table['type']:
        party_type_str = str(party_type)
        for digit in party_type_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<PREFERRED_START_TIME>"])
    # insert the actual value here
    for start_time in time_table_specifications.party_table['preffered_start_time']:
        start_time_str = str(start_time)
        for digit in start_time_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<PREFERRED_END_TIME>"])
    # insert the actul value here
    for end_time in time_table_specifications.party_table['preffered_end_time']:
        end_time_str = str(end_time)
        for digit in end_time_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<PREFERRED_BACK_TO_BACK>"])
    # insert the actual value here
    for back_to_back in time_table_specifications.party_table['preffered_back_to_back']:
        back_to_back_str = str(back_to_back)
        for digit in back_to_back_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<PREFFERED_MAX_HOURS>"])
    # insert the actual value here
    for max_hours in time_table_specifications.party_table['preffered_max_hours']:
        max_hour_str = str(max_hour)
        for digit in max_hour_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Adding session table to the string
    string.append(self.symbol_dictionary["<SESSION_TABLE>"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual value for session table size here
    string.append(time_table_specifications.sessions_table['size'])
    
    string.append(self.symbol_dictionary["<ID>"])
    # insert the actual value here
    for session_id in time_table_specifications.sessions_table['id']:
        session_id_str = str(session_id)
        for digit in session_id_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<TYPE>"])
    # insert the actual value here
    for session_type in time_table_specifications.sessions_table['type']:
        session_type_str = str(session_type)
        for digit in session_type_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<DURATION>"])
    # insert the actual value here
    for duration in time_table_specifications.sessions_table['duration']:
        duration_str = str(duration)
        for digit in duration_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<COURSE>"])
    # insert the actual value here
    for course in time_table_specifications.sessions_table['course']:
        course_str = str(course)
        for digit in course_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<END_TABLE>"])
    
    
    # Adding assignment table to the string
    string.append(self.symbol_dictionary["<ASSIGNMENT_TABLE>"])
    
    string.append(self.symbol_dictionary["<SIZE>"])
    # Insert the actual value for assignment table size here
    string.append(time_table_specifications.assignment_table['size'])
    
    string.append(self.symbol_dictionary["<PARTY_ID>"])
    # insert the actual value here
    for party_id in time_table_specifications.assignment_table['party_id']:
        party_id_str = str(party_id)
        for digit in party_id_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<SESSION_ID>"])
    # insert the actual value here
    for session_id in time_table_specifications.assignment_table['session_id']:
        session_id_str = str(session_id)
        for digit in session_id_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
        
    string.append(self.symbol_dictionary["<PRIORITY>"])
    # insert the actual value here
    for priority in time_table_specifications.assignment_table['priority']:
        priority_str = str(priority)
        for digit in priority_str:
           string.append(int(digit))
        string.append(self.symbol_dictionary[","])
    
    string.append(self.symbol_dictionary["<END_TABLE>"])
    

    # Ending the specifications
    string.append(self.symbol_dictionary["<END_SPECIFICATIONS>"])


    # adding timetable to the string
    string.append(self.symbol_dictionary["<START_TIMETABLE>"])

    # iterating over the sessions in timetable till depth
    for session_id in time_table_specifications.sessions_table['id']:

        # if session id is less than depth we append the timeslot id and venue id from timetable to the string
        if session_id < depth:
            # extracting the timeslot id and appending to the string
            timeslot_id = timetable.timetable[session_id]["timeslot_id"]
            timeslot_id_str = str(timeslot_id)
            for digit in timeslot_id_str:
                string.append(int(digit))
            string.append(self.symbol_dictionary[","])

            # extracting the venue id and appending to the string
            venue_id = timetable.timetable[session_id]["venue_id"]
            venue_id_str = str(venue_id)
            for digit in venue_id_str:
                string.append(int(digit))
            string.append(self.symbol_dictionary["|"])
    
        # else we append tilda to the string, we append them like ~,~ | ~,~ | ... 
        else:
            string.append(self.symbol_dictionary["~"])
            string.append(self.symbol_dictionary[","])
            string.append(self.symbol_dictionary["~"])
            string.append(self.symbol_dictionary["|"])

    # ending the timetable
    string.append(self.symbol_dictionary["<END_TIMETABLE>"])


    # adding options to the string
    string.append(self.symbol_dictionary["<START_OPTIONS>"])

    # extractiong the possible timeslot venue tuple options
    timeslot_venue_tuples = problem[depth]

    # iterating over them and appending them to the string
    for tuple in timeslot_venue_tuples:
    
        # extarcting timeslot id 
        timeslot_id = tuple[0]
        timeslot_id_str = str(timeslot_id)
        for digit in timeslot_id_str:
            string.append(int(digit))
        string.append(self.symbol_dictionary[","])

        # extracting venue id
        venue_id = tuple[1]
        venue_id_str = str(venue_id)
        for digit in venue_id_str:
            string.append(int(digit))
        string.append(self.symbol_dictionary["|"])

    # ending the options
    string.append(self.symbol_dictionary["<END_OPTIONS>"])

    return string

  
  def generate_pretty_string(self, time_table_specifications : TimeTableSpecifications, problem : Problem, timetable : TimeTable, depth : int) -> str:
    numeric_string = self.generate_string(time_table_specifications, problem, timetable, depth)
    
    newline_step = 10
    counter = 0
    pretty_string = ""
    previous_number = False
    
    for i, symbol in enumerate(numeric_string):
        counter += 1
        if (counter >= newline_step or i == len(numeric_string) - 1) and not previous_number:
            pretty_string += "\n"
            counter = 0
        
        if symbol < 10:
            if not previous_number:
                pretty_string += " "
            pretty_string += str(symbol)
            previous_number = True
            continue
            
        if symbol - 10 < len(self.symbols):
            if symbol - 10 < 31:
                pretty_string += "\n" + self.symbols[symbol - 10] + "\n" 
                counter = 0
            else:
                pretty_string += "" + self.symbols[symbol - 10] 

        else:
            print(f"Something went wrong! Symbol ({symbol - 10}) not found in the symbol dictionary (of length: {len(self.symbols)}).")
            pretty_string += " <UNKNOWN>"
        
        previous_number = False
    
    return pretty_string