# Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

CC = g++

OPTIMIZE 	= -c -std=c++0x -g -pedantic -Wall -Wextra -Werror -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Weffc++ -Woverloaded-virtual

DEFINES 	=  

INCLUDES	= -I./include 

CFLAGS 		= $(DEFINES) $(OPTIMIZE) $(INCLUDES)

OBJPATH		= ./obj

EXECUTABLE	= rect_parser

EXECUTABLE_TEST	= rect_parser_test

#----- OBJECTS TO BUILD ------
OBJECTS_MAIN= \
	main.o \

OBJECTS_TEST= \
	main_test.o \
	
OBJECTS= \
	jsonparser.o \
	rectangle.o \
	rectangleoverlaps.o \
	rectanglebuilder.o

.PHONY: all clean

all: $(OBJECTS) $(OBJECTS_MAIN) $(EXECUTABLE)

test: $(OBJECTS) $(OBJECTS_TEST) $(EXECUTABLE_TEST)

clean: 
	rm -f $(OBJECTS)
	rm -f $(OBJECTS_MAIN)
	rm -f $(OBJECTS_TEST)
	rm -f $(EXECUTABLE)
	rm -f $(EXECUTABLE_TEST)

$(EXECUTABLE): $(OBJECTS_MAIN) $(OBJECTS) 
	$(CC) $(OBJECTS_MAIN) $(OBJECTS) $(TARGETARCH) $(LIBTOOLNAME) -o $@

$(EXECUTABLE_TEST): $(OBJECTS_TEST) $(OBJECTS) 
	$(CC) $(OBJECTS_TEST) $(OBJECTS) $(TARGETARCH) $(LIBTOOLNAME) -o $@

main.o : main.cpp
	$(CC) $(CFLAGS) -o $@ $<	
main_test.o : main_test.cpp
	$(CC) $(CFLAGS) -o $@ $<		
jsonparser.o : jsonparser.cpp
	$(CC) $(CFLAGS) -o $@ $<	
rectangle.o : rectangle.cpp
	$(CC) $(CFLAGS) -o $@ $<		
rectangleoverlaps.o : rectangleoverlaps.cpp
	$(CC) $(CFLAGS) -o $@ $<	
rectanglebuilder.o : rectanglebuilder.cpp
	$(CC) $(CFLAGS) -o $@ $<	
