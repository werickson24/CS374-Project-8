#Wil's C Makefile DX+
CC = gcc
FLAGS = -Wall -Wextra -g
EXENAME = memoryengine
CFILES = main.o myalloc.o

all: objects

objects: $(CFILES)
	$(CC) $(FLAGS) -o $(EXENAME) $(CFILES)

#.c file to .o file pattern rule
# forces compiler to spit out warnings in each file
%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@

clean:
	rm -f $(EXENAME) $(CFILES)
