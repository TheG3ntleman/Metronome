from metronome.common.specifications import TimeTableSpecifications

import random
import copy

class TimeTable:
  
  def __init__(self, number_of_sessions : int) -> None:
    self.size = number_of_sessions
    self.timetable = [{"timeslot_id": 0, "venue_id": 0} for i in range(number_of_sessions)]
    
  def schedule_session(self, session_id : int, timeslot_id : int, venue_id : int) -> None:
    self.timetable[session_id] = {"timeslot_id": timeslot_id, "venue_id": venue_id}
    
  def get_session(self, session_id : int) -> dict:
    return self.timetable[session_id]

  def random(self, time_table_specifications : TimeTableSpecifications) -> None:
    for i in range(self.size):
      # Randomly scheduling the sessions.
      random_timetable_index = random.randint(0, time_table_specifications.timeslot_table['size'] - 1)
      random_venue_index = random.randint(0, time_table_specifications.venue_table['size'] - 1)
      self.schedule_session(
        i,
        time_table_specifications.timeslot_table['id'][random_timetable_index],
        time_table_specifications.venue_table['id'][random_venue_index]
      )
  
  def print(self):
    print("Timetable:")
    print("=====================================")
    print("Number of sessions: ", self.size)
    print("=====================================")
    for i in range(self.size):
      print(f"{self.timetable[i]['timeslot_id']:3d}, {self.timetable[i]['venue_id']:3d}")
    
  def __getitem__(self, session_id : int) -> dict:
    return self.timetable[session_id]
  
  def copy(self):
    new_timetable = TimeTable(self.size)
    new_timetable.timetable = copy.deepcopy(self.timetable)
    return new_timetable