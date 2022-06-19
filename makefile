# definizione del compilatore e dei flag di compilazione
# che vengono usate dalle regole implicite
CC=gcc
CFLAGS=-g -Wall -O -std=c99
LDLIBS=-lm -lrt -pthread

# singolo eseguibile da compilare
MAIN=farm
EXECS=

# se si scrive solo make di default compila main
all: $(MAIN) $(EXECS)

farm: farm.o xerrori/xerrori.o funzioni/funzioni.o produttore-consumatore/produttore.o produttore-consumatore/consumatore.o

# target che cancella eseguibili e file oggetto
clean:
	rm -f $(MAIN) $(EXECS) *.o

