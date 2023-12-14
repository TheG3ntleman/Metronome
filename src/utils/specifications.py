import pandas as pd

class Specifications:

    def __init__(self, path):
        self.path = path
        
        # Going to load tables
        self.assignments = pd.read_csv(self.path + "assignments.csv")
        self.parties = pd.read_csv(self.path + "parties.csv")
        self.sessions = pd.read_csv(self.path + "sessions.csv")
        self.venue = pd.read_csv(self.path + "venue.csv")

        # print(self.assignments, self.parties, self.sessions, self.venue)


    def get_sensible_dict_from_pd(self, table):
        new_table = {}
        for key in table.keys():
            new_table[key] = list(table[key].values())
        return new_table

    def get_numerical_tables(self):
        # Must identify critical common columns
        conversion_table = {}

        def get_numerical_table(df):
            raw_table = self.get_sensible_dict_from_pd(df.to_dict())
            numerical_table = {}
            for column in raw_table.keys():
                numerical_table[column] = []
                if column not in conversion_table.keys():
                    conversion_table[column] = {}
                for entry in raw_table[column]:
                    if isinstance(entry, (int, float)):
                        numerical_table[column].append(entry)
                    else:
                        if entry not in conversion_table[column]:
                            conversion_table[column][entry] = len(conversion_table[column])
                        numerical_table[column].append(conversion_table[column][entry])
            return numerical_table

        parties = get_numerical_table(self.parties)
        venues = get_numerical_table(self.venue)
        sessions = get_numerical_table(self.sessions)
        assignments = get_numerical_table(self.assignments)

        return [parties, venues, sessions, assignments], conversion_table
    
    def test(self):
        print(self.get_sensible_dict_from_pd(self.parties.to_dict()))

