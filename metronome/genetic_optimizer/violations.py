from metronome.common import Constraints
#from metronome.common import TimeTableSpecifications

class Violations:

  def __init__(self, time_table_specifications) -> None:
    self.time_table_specifications = time_table_specifications
    self.constraints = Constraints(time_table_specifications= self.time_table_specifications)
  
  def calculate_violations(self, time_table):
    violations = 0
    
    violations += self.constraints.hard_party_conflict(time_table, self.time_table_specifications.number_of_sessions)
    violations += self.constraints.hard_repeated_tuple(time_table, self.time_table_specifications.number_of_sessions)
    violations += self.constraints.hard_venue_capacity(time_table, self.time_table_specifications.number_of_sessions)
    violations += self.constraints.hard_venue_type(time_table, self.time_table_specifications.number_of_sessions)
    violations += self.constraints.hard_max_hours(time_table, self.time_table_specifications.number_of_sessions)
    violations += self.constraints.hard_multi_timeslot(time_table, self.time_table_specifications.number_of_sessions)
    
    return violations