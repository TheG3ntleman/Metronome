from typing import List

class TimeTableSpecifications:

  def __init__(self):
    
    self.number_of_parties = 0
    self.party_ids = []
    self.party_strengths = []
    self.party_max_hours = []
    self.party_type = []
    self.party_preffered_start_time = []
    self.party_preffered_end_time = []
    self.party_preffered_back_to_back = []
    self.party_preffered_max_hours = []
    
    self.number_of_venues = 0
    self.venue_ids = []
    self.venue_types = []
    self.venue_capacities = []
    self.venue_locality = []

    self.number_of_time_slots = 0
    self.time_slot_ids = []
    self.time_slot_days = []

    self.number_of_localities = 0
    self.locality_ids = []
    self.locality_distances = []

    self.number_of_sessions = 0
    self.session_ids = []
    self.session_types = []
    self.session_durations = []
    self.session_courses = []

    self.number_of_assignments = 0
    self.assignment_party_ids = []
    self.assignment_session_ids = []
    self.assignment_priorities = []

    def find_associated_sessions(party_id: int, depth: int) -> List[int]:
      sessions = []
      for i in range(self.number_of_assignments):
        if self.assignment_party_ids[i] == party_id and self.assignment_session_ids[i] <= depth:
          sessions.append(self.assignment_session_ids[i])
      return sessions
