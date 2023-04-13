# Project files

SOURCE_FILES = src/main.c
HEADER_FILES = 
OBJECT_FILES = src/main.o
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


