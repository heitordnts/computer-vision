.SUFFIXES: .c .cpp

CC = gcc
GCC = g++
CFLAGS = `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`

.cpp: 
	$(GCC) -Wall -Wunused -std=c++14 -O2 -g $(CFLAGS) $< -o $@ $(LIBS)

