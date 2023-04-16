# Project files

SOURCE_FILES = src/main.c \
							 src/parser.c \
							
HEADER_FILES = common.h \
							 parser.h \
							 timetable.h

OBJECT_FILES = src/main.o \
							 src/parser.o
EXE_NAME = metronome

# Compiler settings

CC = gcc
CFLAGS = -Wall -Wextra -O3
LFLAGS =

# Recipe stuff

all: build
	./$(EXE_NAME)

build: $(EXE_NAME)

$(EXE_NAME): $(OBJECT_FILES)
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c $(HEADER_FILES)
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm $(EXE_NAME) $(OBJECT_FILES)


