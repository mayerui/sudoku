###########################################
#Makefile for simple programs
###########################################
SRCPATH=./src
INCPATH= -I ./src
LIBPATH= 
OBJPATH= ./obj

#RM=rm -f
RM=del
 
CC=g++
CC_FLAG=-Wall -std=c++11 -ggdb
 
PRG=sudoku
OBJ= block.o scene.o test.o input.o  main.o  
 
$(PRG):$(OBJ)
	$(CC) $(INCPATH) $(LIBPATH) $^ -o $@

%.o : $(SRCPATH)/%.cpp
	$(CC) $(CC_FLAG) $(INCPATH) -c $^ -o $@

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	$(RM) $(OBJ) $(PRG).exe