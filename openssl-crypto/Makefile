# Copyright (C) 2018 Gian Luigi Romita. All rights reserved.

# TO COMPILE => CREATE SYMBOLIC LINK TO THE OPENSSL FOLDER (eg. 'ln -s /your_openssl_path openssl')

CC = g++

OPTIMIZE 	= -c -std=c++0x -g -Werror -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef -Weffc++ -Woverloaded-virtual

CFLAGS 		=  $(OPTIMIZE)

INCLUDES	= -I openssl/include/
LIBS		= -L openssl openssl/libcrypto.a openssl/libssl.a -ldl

EXECUTABLE	= run_sslcrypto

#----- OBJECTS TO BUILD ------
OBJECTS_MAIN= \
	main.o 
	
OBJECTS= \
	sslcrypto.o

.PHONY: all clean

all: $(OBJECTS) $(OBJECTS_MAIN) $(EXECUTABLE)

clean: 
	rm -f $(OBJECTS)
	rm -f $(OBJECTS_MAIN)
	rm -f $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS_MAIN) $(OBJECTS)
	$(CC) $(OBJECTS_MAIN) $(OBJECTS) $(LIBS) $(TARGETARCH) $(LIBTOOLNAME) -o $@

main.o : main.cpp
	$(CC) $(CFLAGS) -o $@ $<		
sslcrypto.o : sslcrypto.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

