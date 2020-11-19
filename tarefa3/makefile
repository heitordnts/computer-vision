.SUFFIXES: .c .cpp

CC = gcc
GCC = g++
CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

.cpp: 
	$(GCC) -Wall -Wunused -std=c++11 -O2 -g $(CFLAGS) $< -o $@ $(LIBS)

