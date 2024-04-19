from specifications import TimeTableSpecifications
from metronome.timetable import TimeTable

class Constraints:
  
  def __init__(self, time_table_specifications : TimeTableSpecifications) -> None:
    self.time_table_specifications = time_table_specifications

  def hard_party_conflict(self, timetable: TimeTable, depth: int) -> int:
    number_of_violations = 0

    for party_id in range(self.time_table_specifications.number_of_parties):
      number_of_sessions = 0
      sessions = []
      sessions = self.time_table_specifications.find_associated_sessions(party_id, depth)

      timeslot_counts = [0] * self.time_table_specifications.number_of_time_slots

      for session in sessions:
        if session > depth:
          continue
        session_info = timetable.get_session_info(session)
        timeslot_counts[session_info["timeslot_id"]] += 1

      for count in timeslot_counts:
        if count > 1:
          number_of_violations += count - 1

    return number_of_violations

  #@staticmethod
  
