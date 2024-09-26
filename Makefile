#simple makefile for g++ compiler

#compiler
CC = g++
FLAGS = -std=c++17 -Wall -Wextra -Werror -O3 -fopenmp

#source files
SRC = $(wildcard *.cpp)
#object files
OBJ = $(SRC:.cpp=.o)
#executable
EXE = main

#default target
all: $(EXE)

#linking
$(EXE): $(OBJ)
	$(CC) $(FLAGS) $^ -o $@

#compiling
%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

#cleaning
clean:
	rm -f $(OBJ) $(EXE)

