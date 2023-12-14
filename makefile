# PROJECT STRUCTURE

ENTRY_POINT_FILE = ./main.py

# Python Related 


# C-Lib Solver Related

SOURCE_FILES = src/metronome.c \
	       src/timetable/specifications.c

HEADER_FILES = src/metronome.c \
	       src/timetable/specifications.c

OBJECT_FILES= src/metronome.o \
	      src/timetable/specifications.o

OUTPUT_LIBRARY_FILE = src/metronome.so

# COMPILATION SETTINGS

CC = gcc
CFLAGS = -Wall -Wextra
LFLAGS = 

# RECIPES

all: metronome
	# This is the default make recepie, it will build all required
	# library files and run the necessary python files to start
	# working on specified data files.
	python $(ENTRY_POINT_FILE)

metronome: $(OUTPUT_LIBRARY_FILE)

$(OUTPUT_LIBRARY_FILE): $(OBJECT_FILES)
	$(CC) $^ $(LFLAGS) -shared -o $@

%.o: %.c %.h
	$(CC) -c -fPIC $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm $(OUTPUT_LIBRARY_FILE) $(OBJECT_FILES) 
