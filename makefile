SOURCE_FILES = src/main.cpp
OBJECT_FILES = src/main.o
EXE_NAME = a 

CC = gcc
C_FLAGS = -Wall -Wextra
L_FLAGS = -lstdc++ -lm

all: $(EXE_NAME)
	./$^

$(EXE_NAME): $(OBJECT_FILES)
	$(CC) $^ $(L_FLAGS) -o $@

%.o: %.c
	$(CC) -c $^ $(C_FLAGS)

.PHONY: clean

clean:
	rm $(EXE_NAME) $(OBJECT_FILES)
