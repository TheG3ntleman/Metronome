# PROJECT STRUCTURE

ENTRY_POINT_FILE = ./main.py
MAIN_DEBUG_FILE = src/main.c

# Python Related 


# C-Lib Solver Related

SOURCE_FILES = src/metronome.c \
               src/timetable/specifications.c \
               src/utils.c \
               src/config.c \
               src/culling.c \
               src/evolution.c \
               src/fitness.c \
               src/operators.c \
               src/population.c \
               src/termination.c

HEADER_FILES = src/metronome.h \
               src/timetable/specifications.h \
               src/utils.h \
               src/config.h \
               src/culling.h \
               src/evolution.h \
               src/fitness.h \
               src/operators.h \
               src/population.h \
               src/termination.h

OBJECT_FILES = src/metronome.o \
               src/timetable/specifications.o \
               src/utils.o \
               src/config.o \
               src/culling.o \
               src/evolution.o \
               src/fitness.o \
               src/operators.o \
               src/population.o \
               src/termination.o

OUTPUT_LIBRARY_FILE = src/metronome.so
OUTPUT_EXECUTABLE_FILE = metronome_debug

# COMPILATION SETTINGS

CC = gcc
CFLAGS = -Wall -Wextra
LFLAGS =

# Debugging and Profiling Flags
DEBUG_FLAGS = -g -O0 -pg

# RECIPES

all: metronome
# This is the default make recipe, it will build all required
# library files and run the necessary python files to start
# working on specified data files.
    python $(ENTRY_POINT_FILE)

metronome: $(OUTPUT_LIBRARY_FILE)

$(OUTPUT_LIBRARY_FILE): $(OBJECT_FILES)
    $(CC) $^ $(LFLAGS) -shared -o $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: LFLAGS += $(DEBUG_FLAGS)
# debug: clean $(OBJECT_FILES)
# Debug mode recipe - compiles to an executable file using MAIN_FILE
    $(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OBJECT_FILES) $(MAIN_FILE) -o $(OUTPUT_EXECUTABLE_FILE)

%.o: %.c %.h
    $(CC) -c -fPIC $(CFLAGS) $< -o $@

.PHONY: clean

clean:
    rm -f $(OUTPUT_LIBRARY_FILE) $(OUTPUT_EXECUTABLE_FILE) $(OBJECT_FILES)

