
CC= egcc
CFLAGS= -Wall
TARGET= example_app

all: bin/main.o  $(TARGET)

$(TARGET): bin/main.o lib/data_structures.a
	$(CC) $(CFLAGS) -o $(TARGET) bin/main.o lib/data_structures.a

bin/main.o: src/main.c
	$(CC) -o bin/main.o -c src/main.c

clean:
	rm -rf bin/* test/*
