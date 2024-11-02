CC= egcc
CFLAGS= -Wall
TARGET= example_app

$(TARGET): main.o ../data_structures/data_structures.a
	$(CC) $(CFLAGS) main.o ../data_structures/data_structures.a -o $(TARGET)

main.o: main.c
	$(CC) -o main.o -c main.c
