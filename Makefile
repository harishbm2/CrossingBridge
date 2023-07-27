CC=gcc
CFLAGS=-I. -lstdc++ -std=c++14
DEPS = bridge.h config.h debug.h hiker.h hiking.h
OBJ = bridge.o config.o debug.o hiker.o hiking.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hike: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)