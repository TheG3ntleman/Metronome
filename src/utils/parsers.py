# We will mostly be dealing with tables as pandas
# dataframes for ease of manipulation.
import pandas as pd

def stage1_parsing(specification_folder_path):
    """
        Loads stage 1 specification by loading
        required csv files as dataframes from
        specification_folder_path and converts them
        to stage 2 specifications.
        
        Saves resultant stage 2 specifications to a
        folder by the name <specification_folder_path>_processed
        and returns folder name.
    """
    print("Attempting stage1 parsing and conversion on"
        f" specifications in {specification_folder_path}")

def stage2_parsing(specification_folder_path):
    """
        Loads stage 2 specifications as dataframes from
        csv(s) and returns those csv(s).

        Currently stage 2 specification require expect 
        4 tables to be present: parties,sessions,venues,
        assignments.
    """
    path_assignment = 'data/sample/assignments.csv'
    assignments_df = pd.read_csv(path_assignment)
    
    
    print("Attempting stage2 parsing and conversion on"
        f" specifications in {specification_folder_path}")

