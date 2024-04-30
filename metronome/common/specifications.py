from typing import List

class TimeTableSpecifications:
  
  def __init__(self):
    self.locality_table = {
      "size" : int(),
      "id": List[int],
      "distance": List[float],
    }
    
    self.venue_table = {
      "size" : int(),
      "id": List[int],
      "type": List[str],
      "capacity": List[int],
      "locality_id": List[int],
    }
    
    self.timeslot_table = {
      "size" : int(),
      "id": List[int],
      "day": List[int]
    }
    
    self.party_table = {
      "size" : int(),
      "id": List[int],
      "strength": List[int],
      "max_hour": List[int],
      "type": List[int],
      "preffered_start_time": List[int],
      "preffered_end_time": List[int],
      "preffered_back_to_back": List[int],
      "preffered_max_hours": List[int]
    }
    
    self.sessions_table = {
      "size": int(),
      "id": List[int],
      "type": List[str],
      "duration": List[int],
      "course": List[int], # Not associated to its own table.
    }
    
    self.assignment_table = {
      "size": int(),
      "party_id": List[int],
      "session_id": List[int],
      "priority": List[int],
    }
    
  def find_associated_sessions(self, party_id : int, depth : int) -> List[int]:
    associated_sessions = []
    for i in range(self.assignment_table['size']):
      # Searching for the sessions that this party must attend.
      if self.assignment_table[i]['party_id'] == party_id:
        # Searching for the sessions which are less than the current depth.
        if self.assignment_table[i]['session_id'] < depth:
          associated_sessions.append(self.assignment_table[i]['session_id'])
    return associated_sessions
  
  def find_associated_parties(self, session_id : int)  -> List[int]:
    associated_parties = []
    for i in range(self.assignment_table['size']):
      # Searching for the parties that must attend this session.
      if self.assignment_table[i]['session_id'] == session_id:
        associated_parties.append(self.assignment_table[i]['party_id'])
    return associated_parties
  
  def print(self):
    # Neatly print the specifications
    print("Locality Table")
    print("Size: ", self.locality_table["size"])
    print("ID: ", self.locality_table["id"])
    print("Distance: ", self.locality_table["distance"])
    print()
    
    print("Venue Table")
    print("Size: ", self.venue_table["size"])
    print("ID: ", self.venue_table["id"])
    print("Type: ", self.venue_table["type"])
    print("Capacity: ", self.venue_table["capacity"])
    print("Locality ID: ", self.venue_table["locality_id"])
    print()
    
    print("Timeslot Table")
    print("Size: ", self.timeslot_table["size"])
    print("ID: ", self.timeslot_table["id"])
    print("Day: ", self.timeslot_table["day"])
    print()
    
    print("Party Table")
    print("Size: ", self.party_table["size"])
    print("ID: ", self.party_table["id"])
    print("Strength: ", self.party_table["strength"])
    print("Max Hour: ", self.party_table["max_hour"])
    print("Type: ", self.party_table["type"])
    print("Preffered Start Time: ", self.party_table["preffered_start_time"])
    print("Preffered End Time: ", self.party_table["preffered_end_time"])
    print("Preffered Back to Back: ", self.party_table["preffered_back_to_back"])
    print("Preffered Max Hours: ", self.party_table["preffered_max_hours"])
    print()
    
    print("Sessions Table")
    print("Size: ", self.sessions_table["size"])
    print("ID: ", self.sessions_table["id"])
    print("Type: ", self.sessions_table["type"])
    print("Duration: ", self.sessions_table["duration"])
    print("Course: ", self.sessions_table["course"])
    print()
    
    print("Assignment Table")
    print("Size: ", self.assignment_table["size"])
    print("Party ID: ", self.assignment_table["party_id"])
    print("Session ID: ", self.assignment_table["session_id"])
    print("Priority: ", self.assignment_table["priority"])
    print()
    
    