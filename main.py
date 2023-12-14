# Will implment argument parser later
# for now I'll just hardcode the problem path

from src import *


if __name__ == "__main__":
    sample_specifications = Specifications("data/sample/")
    [parties, venues, sessions, assignments], conversion_table = sample_specifications.get_numerical_tables()
    solve_timetable(parties, venues, sessions, assignments)


