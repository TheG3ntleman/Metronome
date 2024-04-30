import random
from metronome.common.specifications import TimeTableSpecifications

class MetaSpecifications:
  
  number_of_days = 5
  number_of_timeslots = 7
  
  min_venues = 20
  max_venues = 40
  
  min_lecture_hall_capacity = 50
  max_lecture_hall_capacity = 100
  
  min_class_room_capacity = 20
  max_class_room_capacity = 50
  
  min_tutorial_room_capacity = 10
  max_tutorial_room_capacity = 20
  
  min_auditorium_capacity = 150
  max_auditorium_capacity = 300
    
  min_lab_capacity = 10
  max_lab_capacity = 30
  
  capacity_list = [{min: min_lecture_hall_capacity, max: max_lecture_hall_capacity}, 
                   {min: min_class_room_capacity, max: max_class_room_capacity}, 
                   {min: min_tutorial_room_capacity, max: max_tutorial_room_capacity}, 
                   {min: min_auditorium_capacity, max: max_auditorium_capacity}, 
                   {min: min_lab_capacity, max: max_lab_capacity}]
  
  number_of_student_parties = 30
  number_of_professor_parties = 40

class SyntheticProblemGenerator:
  @staticmethod
  def generate_feasible_specifications() -> TimeTableSpecifications:

    # Making an empty specification object
    specifications = TimeTableSpecifications()

    # Generting locality table, this is fixed for all specifications    
    specifications.locality_table["size"] = 3
    specifications.locality_table["id"] = [0, 1, 2, 3]
    specifications.locality_table["distance"] = [0, 10, 10, 0]
    
    # Generating venue table
    venue_table_size = random.randint(MetaSpecifications.min_venues, MetaSpecifications.max_venues)
    specifications.venue_table["size"] = venue_table_size
    specifications.venue_table["id"] = [i for i in range(venue_table_size)]
    
    # 0: Lecture Hall, 1: Class Room, 2: Tutorial Room, 3: Auditorium, 4: Lab (probably should also account for different types of labs)
    specifications.venue_table["type"] = [random.choice([0, 1, 2, 3, 4]) for i in range(venue_table_size)]
    specifications.venue_table["capacity"] = [ # Capacity is determined based on the type of venue
      random.randint(MetaSpecifications.capacity_list[specifications.venue_table["type"][i]]["min"], MetaSpecifications.capacity_list[specifications.venue_table["type"][i]]["max"]) for i in range(venue_table_size)
    ]
    
    specifications.venue_table["locality_id"] = [random.choice(specifications.locality_table["id"]) for i in range(venue_table_size)]
    
    # Generating timeslot table
    timeslot_table = {
      "size" : 35,
      "id": [i for i in range(MetaSpecifications.number_of_days * MetaSpecifications.number_of_timeslots)],
      "day": [i for  i in range(MetaSpecifications.number_of_days) for i in range(MetaSpecifications.number_of_timeslots)]
    }
    
    # Generating the parties table
    
