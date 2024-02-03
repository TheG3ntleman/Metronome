# PROJECT STRUCTURE

ENTRY_POINT_FILE = ./main.py
MAIN_DEBUG_FILE = src/main.c

# Python Related 


# C-Lib Solver Related

SOURCE_FILES = src/metronome.c \
	       src/timetable/specifications.c \
	       src/timetable/utils.c \
	       src/timetable/config.c \
	       src/timetable/culling.c \
	       src/timetable/evolution.c \
	       src/timetable/fitness.c \
	       src/timetable/operators.c \
	       src/timetable/population.c \
	       src/timetable/termination.c 

HEADER_FILES = src/metronome.h \
	       src/timetable/specifications.h \
	       src/timetable/utils.h \
	       src/timetable/config.h \
	       src/timetable/culling.h \
	       src/timetable/evolution.h \
	       src/timetable/fitness.h \
	       src/timetable/operators.h \
	       src/timetable/population.h \
	       src/timetable/termination.h

OBJECT_FILES= src/metronome.o \
	      src/timetable/specifications.o \
	      src/timetable/utils.o \
	      src/timetable/config.o \
	      src/timetable/culling.o \
	      src/timetable/evolution.o \
	      src/timetable/fitness.o \
	      src/timetable/operators.o \
	      src/timetable/population.o \
	      src/timetable/termination.o 

OUTPUT_LIBRARY_FILE = src/metronome.so
OUTPUT_DEBUG_EXECUTABLE_FILE = metronome_debug

# COMPILATION SETTINGS

CC = gcc
CFLAGS = -Wall -Wextra
LFLAGS =
DEBUG_FLAGS = -g -O0 -pg

# RECIPES

all: metronome
# This is the default make recepie, it will build all required
# library files and run the necessary python files to start
# working on specified data files.
	python $(ENTRY_POINT_FILE)

metronome: $(OUTPUT_LIBRARY_FILE)

$(OUTPUT_LIBRARY_FILE): $(OBJECT_FILES)
	$(CC) $^ $(LFLAGS) -shared -o $@

debug: $(OBJECT_FILES) 
	$(CC) $(DEBUG_FLAGS) $(CFLAGS) $(LFLAGS) $^ src/main.c -o $(OUTPUT_DEBUG_EXECUTABLE_FILE)

%.o: %.c %.h
	$(CC) -c -fPIC $(DEBUG_FLAGS) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm $(OUTPUT_LIBRARY_FILE) $(OUTPUT_DEBUG_EXECUTABLE_FILE) $(OBJECT_FILES) gmon.out 
