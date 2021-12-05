# Makefile for Library App
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
# LIB_FLAGS 
 
# ****************************************************
# Targets needed to bring the executable up to date

all: main.o library_admin_helper.o library_util.o
	$(CC) $(CFLAGS) main.o library_admin_helper.o library_util.o -o runner -lsqlite3

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -lsqlite3

library_admin_helper.o: library_admin_helper.cpp library_admin_helper.h
	$(CC) $(CFLAGS) -c library_admin_helper.cpp -lsqlite3

library_util.o: library_util.cpp library_util.h
	$(CC) $(CFLAGS) -c library_util.cpp

clean:
	rm -rf *.o runner