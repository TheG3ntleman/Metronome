# Project files

SOURCE_FILES = src/main.c \
							 src/common/common.c \
							 src/common/string.c \
							 src/common/file.c \
							 src/timetable.c \
							 src/parser.c \
							 src/gen.c

HEADER_FILES = src/common/common.h \
							 src/common/string.h \
							 src/common/file.h \
							 src/parser.h \
							 src/gen.h

OBJECT_FILES = src/main.o \
							 src/common/common.o \
							 src/common/string.o \
							 src/common/file.o \
							 src/timetable.o \
							 src/parser.o \
							 src/gen.o

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


