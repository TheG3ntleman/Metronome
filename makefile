# PROJECT STRUCTURE

ENTRY_POINT_FILE = src/main.py

# Python Related 

# C-Lib Solver Related

# COMPILATION SETTINGS

# RECIPES

all: 
	# This is the default make recepie, it will build all required
	# library files and run the necessary python files to start
	# working on specified data files.
	python $(ENTRY_POINT_FILE)

