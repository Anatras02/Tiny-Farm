CC=gcc
CFLAGS=-g -Wall -O -std=gnu99
LDLIBS=-lm -lrt -pthread

# eseguibili da costruire
EXECS=farm

.PHONY: clean

# di default make cerca di realizzare il primo target
all: $(EXECS)

# non devo scrivere il comando associato ad ogni target
# perché il default di make in questo caso va bene

farm: farm.o
