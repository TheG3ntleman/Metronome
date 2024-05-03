import random
# from metronome.common.specifications import TimeTableSpecifications
from metronome.common.specifications import TimeTableSpecifications
from metronome.common.violations import Violations
import numpy as np  # type: ignore

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

    min_lab_capacity = 150 # Changed from 10 to 150
    max_lab_capacity = 300 # Changed from 30 to 300

    capacity_list = [{'min': min_lecture_hall_capacity, 'max': max_lecture_hall_capacity}, 
                    {'min': min_class_room_capacity, 'max': max_class_room_capacity}, 
                    {'min': min_tutorial_room_capacity, 'max': max_tutorial_room_capacity}, 
                    {'min': min_auditorium_capacity, 'max': max_auditorium_capacity}, 
                    {'min': min_lab_capacity, 'max': max_lab_capacity}]

    number_of_student_parties = 30
    number_of_professor_parties = 40

    min_student_party_strength = 20
    max_student_party_strength = 80

    preferred_start_time_lower_bound = 0  # 8:30 - 9:30
    preferred_start_time_upper_bound = 2  # 10:30 - 11:30

    preferred_end_time_lower_bound = 4  # 3:30 - 4:30
    preferred_end_time_upper_bound = 6  # 5:30 - 6:30

    preferred_max_hours_lower_bound = 2
    preferred_max_hours_upper_bound = 6

    preferred_back_to_back_lower_bound = 2
    preferred_back_to_back_upper_bound = 3


    min_student_party_strength = 20
    max_student_party_strength = 80

    preferred_start_time_lower_bound = 0  # 8:30 - 9:30
    preferred_start_time_upper_bound = 2  # 10:30 - 11:30

    preferred_end_time_lower_bound = 4  # 3:30 - 4:30
    preferred_end_time_upper_bound = 6  # 5:30 - 6:30

    preferred_max_hours_lower_bound = 2
    preferred_max_hours_upper_bound = 6

    preferred_back_to_back_lower_bound = 2
    preferred_back_to_back_upper_bound = 3

class SyntheticProblemGenerator:

    @staticmethod
    def generate_feasible_specifications() -> TimeTableSpecifications:

        # Making an empty specification object
        specifications = TimeTableSpecifications()
        # Making an empty specification object
        specifications = TimeTableSpecifications()

        # Generting locality table, this is fixed for all specifications
        specifications.locality_table["size"] = 2
        specifications.locality_table["id"] = [0, 1, 2]
        specifications.locality_table["distance"] = [0, 10, 0]

        # Generating venue table
        venue_table_size = random.randint(
            MetaSpecifications.min_venues,
            MetaSpecifications.max_venues
        )
        specifications.venue_table["size"] = venue_table_size
        specifications.venue_table["id"] = [i for i in range(venue_table_size)]

        # 0: Lecture Hall, 1: Class Room, 2: Tutorial Room, 3: Auditorium, 4: Lab (probably should also account for different types of labs)
        specifications.venue_table["type"] = [random.choice(
            [0, 1, 2, 3, 4]) for i in range(venue_table_size)]
        specifications.venue_table["capacity"] = [  # Capacity is determined based on the type of venue
            random.randint(MetaSpecifications.capacity_list[specifications.venue_table["type"][i]]["min"], MetaSpecifications.capacity_list[specifications.venue_table["type"][i]]["max"]) for i in range(venue_table_size)
        ]

        specifications.venue_table["locality_id"] = [random.choice(
            specifications.locality_table["id"]) for i in range(venue_table_size)]

        # Generating timeslot table
        days: list = []
        cnt: int = -1
        for i in range(MetaSpecifications.number_of_days):
            cnt += 1
            for j in range(MetaSpecifications.number_of_timeslots):
                days.append(cnt)

        timeslot_table = {
            "size": 35,
            "id": [i for i in range(MetaSpecifications.number_of_days * MetaSpecifications.number_of_timeslots)],
            "day": days
        }
        
        specifications.timeslot_table['size'] = timeslot_table['size']
        specifications.timeslot_table['id'] = timeslot_table['id']
        specifications.timeslot_table['day'] = timeslot_table['day']

        # Generating the parties table
        # also, need to convert the entire thing to in line for loops
        specifications.party_table["size"] = MetaSpecifications.number_of_student_parties + \
            MetaSpecifications.number_of_professor_parties
        specifications.party_table["id"] = [i for i in range(
            MetaSpecifications.number_of_student_parties + MetaSpecifications.number_of_professor_parties)]

        specifications.party_table["type"] = []
        for i in range(MetaSpecifications.number_of_student_parties):
            specifications.party_table["type"].append(0)
        for i in range(MetaSpecifications.number_of_professor_parties):
            specifications.party_table["type"].append(1)

        specifications.party_table["strength"] = []

        for i in range(MetaSpecifications.number_of_student_parties + MetaSpecifications.number_of_professor_parties):
            if specifications.party_table["type"][i] == 0:
                specifications.party_table["strength"].append(random.randint(
                    MetaSpecifications.min_student_party_strength, MetaSpecifications.max_student_party_strength))
            else:
                specifications.party_table["strength"].append(1)

        specifications.party_table["max_hour"] = []
        for i in range(MetaSpecifications.number_of_student_parties + MetaSpecifications.number_of_professor_parties):
            if specifications.party_table["type"][i] == 0:
                specifications.party_table["max_hour"].append(
                    random.randint(2, 6))
            else:
                specifications.party_table["max_hour"].append(
                    random.randint(2, 3))

        specifications.party_table["preffered_start_time"] = []
        specifications.party_table["preffered_end_time"] = []
        specifications.party_table["preffered_max_hours"] = []
        specifications.party_table["preffered_back_to_back"] = []

        for i in range(MetaSpecifications.number_of_student_parties + MetaSpecifications.number_of_professor_parties):
            specifications.party_table["preffered_start_time"].append(random.randint(
                MetaSpecifications.preferred_start_time_lower_bound, MetaSpecifications.preferred_start_time_upper_bound))
            specifications.party_table["preffered_end_time"].append(random.randint(
                MetaSpecifications.preferred_end_time_lower_bound, MetaSpecifications.preferred_end_time_upper_bound))
            specifications.party_table["preffered_max_hours"].append(random.randint(
                MetaSpecifications.preferred_max_hours_lower_bound, MetaSpecifications.preferred_max_hours_upper_bound))
            specifications.party_table["preffered_back_to_back"].append(random.randint(
                MetaSpecifications.preferred_back_to_back_lower_bound, MetaSpecifications.preferred_back_to_back_upper_bound))

        # Generating the matrix from Eclair field
        number_of_blocks = 10
        matrix = np.zeros((specifications.timeslot_table['size'], venue_table_size, specifications.party_table['size']))  # (Timeslots, Venues, Parties)
        EclairField_object = EclairField(
            number_of_blocks, specifications, matrix)
        block_list = EclairField_object.generate_blocks()
        matrix = EclairField_object.generate_matrix(
            block_list[0], block_list[1], block_list[2])

        number_of_sessions: int = 0

        number_of_sessions_set: set = set()
        corresponding_venues: list = []

        session_durations: list = []
        number_of_assignments: int = 0
        for i in range(matrix.shape[0]):  # Timeslots
            for j in range(matrix.shape[1]):  # Venues
                for k in range(matrix.shape[2]):  # Parties
                    if matrix[i][j][k] != 0:
                        number_of_assignments += 1
                        if matrix[i][j][k] not in number_of_sessions_set:
                            number_of_sessions_set.add(matrix[i][j][k])
                            corresponding_venues.append(j)
                            number_of_sessions += 1
                        # this has to be changed, not possibly correct -> done
                            if i+1 < matrix.shape[0] and matrix[i][j][k] < 0 and matrix[i][j][k] == matrix[i+1][j][k]: # need to change this -> done
                                session_durations.append(2)  # if it's a lab
                            else:
                                session_durations.append(1)  # if it's a lecture

        # number_of_sessions = len(number_of_sessions)

        specifications.sessions_table["size"] = number_of_sessions
        specifications.sessions_table["id"] = [i for i in range(number_of_sessions)]
        specifications.sessions_table["type"] = []
        for q in range(len(corresponding_venues)):
            specifications.sessions_table["type"].append(specifications.venue_table["type"][corresponding_venues[q]])

        specifications.sessions_table["duration"] = session_durations
        specifications.sessions_table["course"] = [random.randint(0, 7) for i in range(number_of_sessions)]

        # Generating the assignment table
        specifications.assignment_table["size"] = number_of_assignments

        specifications.assignment_table["party_id"] = []
        specifications.assignment_table["session_id"] = []
        specifications.assignment_table["priority"] = []
        
        session_id: int = 0
        for i in range(matrix.shape[0]): # Timeslot
            for j in range(matrix.shape[1]): # Venue
                cnt: int = 0
                for k in range(matrix.shape[2]): # Party
                    if matrix[i][j][k] != 0:
                        specifications.assignment_table["party_id"].append(k)
                        specifications.assignment_table["session_id"].append(session_id)
                        specifications.assignment_table["priority"].append(1)
                        cnt += 1
                if cnt != 0:  
                  session_id += 1


        return specifications


class EclairField:

    def __init__(self, number_of_blocks, TimeTableSpecifications, matrix) -> None:
        self.number_of_blocks = number_of_blocks
        self.TimeTableSpecifications = TimeTableSpecifications
        self.matrix = matrix

    def generate_blocks(self):
        blocks: list = []
        block_repeat: list = []
        capacities: list = []
        minimum_number_parties_in_the_block = 2
        maximum_number_parties_in_the_block = 7

        minimum_number_of_block_repeat = 2  # 2 times in a week
        maximum_number_of_block_repeat = 4  # 4 times in a week

        for i in range(self.number_of_blocks):
            block = []
            capacity = 0
            block_repeats = random.randint(
                minimum_number_of_block_repeat, maximum_number_of_block_repeat)
            number_of_parties = random.randint(
                minimum_number_parties_in_the_block, maximum_number_parties_in_the_block)

            set_of_parties = set()

            for j in range(number_of_parties):  # minium 2 parties -> lecturer and 1 party
                if j == 0:
                    block.append(random.randint(29, 69))  # lecturer party
                else:
                    # Should use a set, to avoid duplicates -> done
                    party_id = random.randint(0, 28)
                    while True:
                        if party_id not in set_of_parties:
                            break
                        party_id = random.randint(0, 28)

                    set_of_parties.add(party_id)
                    block.append(party_id)
                    print("Party ID: ", party_id)
                    capacity += (
                        self.TimeTableSpecifications.party_table["strength"][party_id])
                    # capacity += 10

            blocks.append(block)  # should change it -> done
            block_repeat.append(block_repeats)
            # combined strength of all the parties in the block, for finding the venue
            capacities.append(capacity)

        for i in range(self.number_of_blocks):
            print()
            print("Block: ", blocks[i])
            print("Block Repeat: ", block_repeat[i])
            print("Capacity: ", capacities[i])
            print()

      # Need to change the capcity, but blocks seems perfect

        return [blocks, block_repeat, capacities]

    def generate_matrix(self, blocks, block_repeat, capacities):

        shape = np.shape(self.matrix)  # (Timeslots, Venues, Parties)
        print("Shape: ", shape)
        number_of_timeslots = shape[0]
        number_of_blocks = len(blocks)
        timeslot_flag: int = number_of_timeslots
        session_id: int = 1
        possible_session_is_lab: int = -1 # for a lab session, we are using negative values to make it consider as a single session

        # total number of parties in the blocks
        # set
        party_sets: set = set()
        for i in range(len(blocks)):
            for j in range(len(blocks[i])):
                party_sets.add(blocks[i][j])

        party_list: list = list(party_sets)

        # check whether the timeslot is there or not
        is_the_timeslot_there_or_not: dict = {}

        for i in range(len(party_list)):
            val: set = set()
            key: int = party_list[i]
            is_the_timeslot_there_or_not[key] = val

        for j in range(number_of_blocks):  # for the block
            print("Capacity: ", capacities[j])  # This is perfect
            print("Block Repeat: ", block_repeat[j])
            # for the block_repeat, number of times the block is repeated in the matrix

            for _ in range(block_repeat[j]):

                # for every repeatition, the capacity remains the same
                capacity = capacities[j]
                # for every repeatition, the venue also remains the same
                venue_id, venue_type = self.find_the_venue(capacity)
                print("Venue ID: ", venue_id, "Venue Type: ", venue_type)
                if venue_id == -1 and venue_type == -1:

                    # There's no venue available for the block
                    # We break it, cause it's not possible to find a venue for the block
                    print("No Venue Available")  # fine
                    break

                if venue_type == 4:
                    # also, if the venue-type is 4, then we need two continous time slots -> (lab)
                    print("Venue Type: Lab")

                    check = -1
                    possible_timeslot: list = []
                    for i in range(number_of_timeslots):
                        checker = True
                        for party in blocks[j]:
                            if is_the_timeslot_there_or_not[party].__contains__(i) == True:
                                checker = False
                                break
                        if checker == True:
                            check = i
                            possible_timeslot.append(i)

                    if len(possible_timeslot) == 0:  # No timeslot available for the block
                        break

                    timeslot_choosen: int = -1

                    print()
                    print("Possible Timeslots: ", possible_timeslot)
                    print()

                    for timeslots in possible_timeslot:
                        print()
                        print("Timeslots: ", timeslots)
                        print()
                        if timeslots in possible_timeslot and (timeslots + 1) in possible_timeslot and timeslots // 7 == (timeslots + 1) // 7:
                            timeslot_choosen = timeslots
                            print("Timeslot Choosen of type Lab: ", timeslot_choosen)
                            break
                    if timeslot_choosen == -1:  # no possible timeslot for a lab
                        print("No Possible Timeslot for a Lab")
                        break
                    
                    for party in blocks[j]:
                        self.matrix[timeslot_choosen][venue_id][party] = possible_session_is_lab
                        self.matrix[timeslot_choosen +
                                    1][venue_id][party] = possible_session_is_lab
                        is_the_timeslot_there_or_not[party].add(
                            timeslot_choosen)
                        is_the_timeslot_there_or_not[party].add(
                            timeslot_choosen + 1)

                    session_id -= 2


                else:

                    check = -1
                    possible_timeslot: list = []
                    for i in range(number_of_timeslots):
                        checker = True
                        for party in blocks[j]:
                            if is_the_timeslot_there_or_not[party].__contains__(i) == True:
                                checker = False
                                break
                        if checker == True:
                            check = i
                            possible_timeslot.append(i)

                    if len(possible_timeslot) == 0:  # No timeslot available for the block
                        break

                    timeslot_choosen = random.choice(possible_timeslot)
                    # print("Timeslot Choosen: ", timeslot_choosen)
                    for party in blocks[j]:
                        print("Party: ", party)
                        print("Timeslot Choosen: ", timeslot_choosen)
                        print("Venue ID: ", venue_id)
                        self.matrix[timeslot_choosen][venue_id][party] = session_id
                        is_the_timeslot_there_or_not[party].add(
                            timeslot_choosen)

                    session_id += 1


        print("\n\n\nMatrix:")
        print(self.matrix, "\n\n")
        return self.matrix

    def find_the_venue(self, capacity) -> list:
        venue_table = self.TimeTableSpecifications.venue_table
        if random.random() < 0.8:
            for i in range(venue_table["size"]):
                if venue_table["capacity"][i] >= capacity:
                    # print("Venue ID: ", venue_table["id"][i])
                    return [venue_table["id"][i], venue_table["type"][i]]
        else:
            # In reverse direction
            for i in range(venue_table["size"]-1, -1, -1):
                if venue_table["capacity"][i] >= capacity:
                    # print("Venue ID: ", venue_table["id"][i])
                    return [venue_table["id"][i], venue_table["type"][i]]

        return [-1, -1]
        # We basically have to find a venue for the party blocks and then assign all the parties to that venue at a timeslot

""""
if __name__ == "__main__":

    # TimeTableSpecifications object
    a = SyntheticProblemGenerator.generate_feasible_specifications()


    # print(a.locality_table)
    # print(a.venue_table)
    # print(a.timeslot_table)
    # print(a.party_table)
    # print(a.sessions_table)
    # print(a.assignment_table)

    # From the violations class, we can calculate the violations
    b = Violations(a)
    print(b.calculate_violations(a))
"""
    
