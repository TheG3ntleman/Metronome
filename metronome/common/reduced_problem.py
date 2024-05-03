from metronome.common.timetable import TimeTable

import numpy as np

class Problem:
  
  def __init__(self, number_of_sessions, options) -> None:
    self.number_of_sessions = number_of_sessions
    self.options = options
  
  def __getitem__(self, key : int):
    return self.options[key]
  
  def print(self):
    print("Number of sessions:", self.number_of_sessions)
    
    for session_id in range(self.number_of_sessions):
      print("Session", session_id, ":", self.options[session_id])
    
  
class MCTSTupleFrequencyProblem(Problem):
  
  def __init__(self, population, number_of_timeslots, number_of_venues, top_k = 5):
    options = []
    
    for session_id in range(population[0].size):
      # Frequency analysis over tuples
      frequency_table = np.zeros((number_of_timeslots, number_of_venues))
      for timetable_id in range(len(population)):
        session_info = population[timetable_id].get_session(session_id)
        frequency_table[session_info["timeslot_id"], session_info["venue_id"]] += 1
      
      # We then proceed to find the indices of the top k frequent tuples
      top_k_tuples = []
      for _ in range(top_k):
        max_tuple = np.unravel_index(np.argmax(frequency_table), frequency_table.shape)
        top_k_tuples.append(max_tuple)
        frequency_table[max_tuple] = -1
      
      options.append(top_k_tuples)
    
    super().__init__(population[0].size, options)
  
  def print(self):
    print("Tuple Frequency Type Problem")
    super().print()
    
    
class MCTSElementFrequencyProblem(Problem):
  def __init__(self, population, number_of_timeslots, number_of_venues, top_k = 5):
    options = []
    
    for session_id in range(population[0].size):
      # Frequency analysis over timeslots  and venue seperately
      timeslot_frequency = np.zeros(number_of_timeslots)
      venue_frequency = np.zeros(number_of_venues)
      
      for timetable_id in range(len(population)):
        session_info = population[timetable_id].get_session(session_id)
        timeslot_frequency[session_info["timeslot_id"]] += 1
        venue_frequency[session_info["venue_id"]] += 1
      
      # We then proceed to find the indices of the top k frequent elements
      top_k_timeslots = np.argsort(timeslot_frequency)[-top_k:]
      top_k_venues = np.argsort(venue_frequency)[-top_k:]
      
      # We now generate tuples by combining the top k timeslots and venues
      top_k_elements = []
      for timeslot_id in top_k_timeslots:
        for venue_id in top_k_venues:
          top_k_elements.append((timeslot_id, venue_id))
      
      options.append(top_k_elements)
    
    super().__init__(population[0].size, options)
  
  def print(self):
    print("Element Frequency Type Problem")
    super().print()
    