# Will implment argument parser later
# for now I'll just hardcode the problem path

from utils import *


if __name__ == "__main__":
    sample_specifications = Specifications("../data/sample/")
    tables, conversion_table = sample_specifications.get_numerical_tables()
    with open("output.txt", "w") as file:
        for table in tables:
            file.writelines(str(table) + "\n\n")
        file.writelines("\n\nCONVERSION TABLE\n\n")
        for key in conversion_table.keys():
            file.writelines(str(key) + ":\n" + str(conversion_table[key]) + "\n\n")
    sample_specifications.test()


