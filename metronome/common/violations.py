from metronome.common import Constraints
from metronome.common import TimeTableSpecifications

#boolean variables to enable soft constraints
soft_constraint_enable = True

class Violations:

  def __init__(self, time_table_specifications : TimeTableSpecifications) -> None:
    self.time_table_specifications : TimeTableSpecifications = time_table_specifications
    self.constraints = Constraints(time_table_specifications= self.time_table_specifications)
  
  def calculate_violations(self, time_table):
    violations = 0
    soft_violations = 0
    
    hard_party_violations = self.constraints.hard_party_conflict(time_table, self.time_table_specifications.session_table['size'])#number_of_sessions)
    violations += hard_party_violations
    
    hard_repeat_violations = self.constraints.hard_repeated_tuple(time_table, self.time_table_specifications.session_table['size'])#number_of_sessions)
    violations += hard_repeat_violations
    
    hard_venue_capacity_violations = self.constraints.hard_venue_capacity(time_table, self.time_table_specifications.session_table['size'])#number_of_sessions)
    violations += hard_venue_capacity_violations
    
    hard_venue_type_violations = 3 * self.constraints.hard_venue_type(time_table, self.time_table_specifications.session_table['size'])
    violations += hard_venue_type_violations
    
    hard_max_hours_violations = self.constraints.hard_max_hours(time_table, self.time_table_specifications.session_table['size'])
    violations += hard_max_hours_violations
    
    hard_multi_timeslot_violations = 5 * self.constraints.hard_multi_timeslot(time_table, self.time_table_specifications.session_table['size'])
    violations += hard_multi_timeslot_violations
    
    if not soft_constraint_enable:
      return [violations, hard_party_violations, hard_repeat_violations, hard_venue_capacity_violations, hard_venue_type_violations, hard_max_hours_violations, hard_multi_timeslot_violations]
    
    if soft_constraint_enable:
      soft_travel_time_violations = self.constraints.soft_travel_time(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_travel_time_violations

      soft_chunking_violations = self.constraints.soft_chunking(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_chunking_violations

      soft_room_utilization_violations = self.constraints.soft_room_utilization(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_room_utilization_violations

      soft_extreme_time_violations = self.constraints.soft_extreme_time(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_extreme_time_violations

      soft_room_capacity_violations = self.constraints.soft_room_capacity_utilization(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_room_capacity_violations

      soft_coommon_timeslot_empty_violations = self.constraints.soft_common_timeslot_empty(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_coommon_timeslot_empty_violations

      soft_minimize_back_to_back_violations = self.constraints.soft_minimize_back_to_back(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_minimize_back_to_back_violations

      soft_repeated_course_sessions_violations = self.constraints.soft_minimize_back_to_back(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_repeated_course_sessions_violations

      soft_sessions_well_distributed_violations = self.constraints.soft_sessions_well_distributed(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_sessions_well_distributed_violations

      soft_lab_after_lecture_violations = self.constraints.soft_lab_after_lecture(time_table, self.time_table_specifications.session_table['size'])
      soft_violations += soft_lab_after_lecture_violations
    
      return [3*violations + soft_violations, hard_party_violations, hard_repeat_violations, hard_venue_capacity_violations, hard_venue_type_violations, hard_max_hours_violations, hard_multi_timeslot_violations, soft_travel_time_violations, soft_chunking_violations, soft_room_utilization_violations, soft_extreme_time_violations, soft_room_capacity_violations, soft_coommon_timeslot_empty_violations, soft_minimize_back_to_back_violations, soft_repeated_course_sessions_violations, soft_sessions_well_distributed_violations, soft_lab_after_lecture_violations]
  
  def get_violation_list(self):
    if soft_constraint_enable:
      return ["Party Violations (Hard)", "Repeat Violations (Hard)", "Venue Capacity Violations (Hard)", "Venue Type Violations (Hard)", "Max Hours Violations (Hard)", "Multi Timeslot Violations (Hard)", "Travel Time Violations (Soft)", "Chunking Violations (Soft)", "Room Utilization Violations (Soft)", "Extreme Time Violations (Soft)", "Room Capacity Violations (Soft)", "Common Timeslot Empty Violations (Soft)", "Minimize Back to Back Violations (Soft)", "Repeated Course Sessions Violations (Soft)", "Sessions Well Distributed Violations (Soft)", "Lab After Lecture Violations (Soft)"]
    else:
      return ["Party Violations", "Repeat Violations", "Venue Capacity Violations", "Venue Type Violations", "Max Hours Violations", "Multi Timeslot Violations"]

  def get_number_of_violations(self):
    return 16 if soft_constraint_enable else 6