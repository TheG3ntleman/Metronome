# Project structure

SOURCE_FILES = src/main.c 
HEADER_FILES =  
OBJECT_FILES = src/main.o 

EXECUTABLE_NAME = metronome

# Compilation options
CC = clang
LFLAGS = 
CFLAGS = -Wall -Wextra

# Recipe

all: build
	./$(EXECUTABLE_NAME)


build: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME): $(OBJECT_FILES)
	$(CC) $(LFLAGS) -o $@ $^

%.o: %.c $(HEADER_FILES)
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm $(EXECUTABLE_NAME)
	rm $(OBJECT_FILES)
