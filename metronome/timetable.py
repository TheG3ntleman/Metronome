import random
from metronome.common import TimeTableSpecifications
class TimeTable:
  
  def __init__(self, number_of_sessions):
    self.number_of_sessions = number_of_sessions
    self.timetable = [{"timeslot_id": 0, "venue_id":0} for i in range(number_of_sessions)]
    
  def schedule_session(self, session_id, timeslot_id, venue_id):
    self.timetable[session_id]["timeslot_id"] = timeslot_id
    self.timetable[session_id]["venue_id"] = venue_id
  
  def get_session_info(self, session_id):
    return self.timetable[session_id]
  
  def randomly_initialize(self, time_table_specifications : TimeTableSpecifications) -> None:
    for i in range(self.number_of_sessions):
      self.schedule_session(
        i, 
        time_table_specifications.time_slot_ids[random.randint(0, time_table_specifications.number_of_time_slots - 1)], 
        time_table_specifications.venue_ids[random.randint(0, time_table_specifications.number_of_venues - 1)]
        )
  
  def print(self):
    print("TIME TABLE")
    print("\tNumber of sessions:", self.number_of_sessions)
    print("\nTuples:")
    for session_id in range(self.number_of_sessions):
      print("\t" + str(self.timetable[session_id]["timeslot_id"]), " - ", self.timetable[session_id]["venue_id"])
  
  def __getitem__(self, key):
    return self.timetable[key]
    
  