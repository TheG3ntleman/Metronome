from metronome.common import Constraints
#from metronome.common import TimeTableSpecifications

class Violations:

  def __init__(self, time_table_specifications) -> None:
    self.time_table_specifications = time_table_specifications
    self.constraints = Constraints(time_table_specifications= self.time_table_specifications)
  
  def calculate_violations(self, time_table):
    violations = 0
    
    hard_party_violations = 5 * self.constraints.hard_party_conflict(time_table, self.time_table_specifications.number_of_sessions)
    violations += hard_party_violations
    
    hard_repeat_violations = self.constraints.hard_repeated_tuple(time_table, self.time_table_specifications.number_of_sessions)
    violations += hard_repeat_violations
    
    hard_venue_capacity_violations = self.constraints.hard_venue_capacity(time_table, self.time_table_specifications.number_of_sessions)
    violations += hard_venue_capacity_violations
    
    hard_venue_type_violations = 3 * self.constraints.hard_venue_type(time_table, self.time_table_specifications.number_of_sessions)
    violations += hard_venue_type_violations
    
    hard_max_hours_violations = self.constraints.hard_max_hours(time_table, self.time_table_specifications.number_of_sessions)
    violations += hard_max_hours_violations
    
    hard_multi_timeslot_violations = 5 * self.constraints.hard_multi_timeslot(time_table, self.time_table_specifications.number_of_sessions)
    violations += hard_multi_timeslot_violations
    
    return [violations, hard_party_violations, hard_repeat_violations, hard_venue_capacity_violations, hard_venue_type_violations, hard_max_hours_violations, hard_multi_timeslot_violations]
  
  def get_violation_list(self):
    return ["Party Violations", "Repeat Violations", "Venue Capacity Violations", "Venue Type Violations", "Max Hours Violations", "Multi Timeslot Violations"]

  def get_number_of_violations(self):
    return 6