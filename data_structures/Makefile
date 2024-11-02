
CC= egcc
CFLAGS= -Wall -lpthread
TARGET= data_structures.a


all: bin/dict.o bin/list.o bin/list.test.o bin/str.o bin/str.test.o bin/dict.test.o bin/set.o bin/heap.test.o bin/memory.o bin/set.test.o bin/item.o bin/test_utils.o bin/memory.test.o bin/dict_conn.o bin/dict_perf.test.o bin/list_perf.test.o bin/list_conn.o bin/linked_list.test.o bin/list_extended.o bin/dict_extended.o bin/set_extended.o bin/heap_conn.o bin/utils.o bin/set_conn.o bin/linked_list.o bin/heap.o bin/heap_perf.test.o bin/utils.test.o bin/concurrency.test.o test/list test/str test/dict test/heap test/set test/memory test/dict_perf test/list_perf test/linked_list test/heap_perf test/utils test/concurrency $(TARGET)

$(TARGET): bin/dict.o bin/list.o bin/str.o bin/set.o bin/memory.o bin/item.o bin/test_utils.o bin/dict_conn.o bin/list_conn.o bin/list_extended.o bin/dict_extended.o bin/set_extended.o bin/heap_conn.o bin/utils.o bin/set_conn.o bin/linked_list.o bin/heap.o
	ar -r $(TARGET) bin/dict.o bin/list.o bin/str.o bin/set.o bin/memory.o bin/item.o bin/test_utils.o bin/dict_conn.o bin/list_conn.o bin/list_extended.o bin/dict_extended.o bin/set_extended.o bin/heap_conn.o bin/utils.o bin/set_conn.o bin/linked_list.o bin/heap.o

test/list: bin/list.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/utils.o bin/list.o
	$(CC) $(CFLAGS) bin/list.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/utils.o bin/list.o -o test/list

test/str: bin/str.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/utils.o bin/list.o
	$(CC) $(CFLAGS) bin/str.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/utils.o bin/list.o -o test/str

test/dict: bin/dict.test.o bin/str.o bin/test_utils.o bin/memory.o bin/dict_extended.o bin/item.o bin/utils.o bin/dict.o bin/list.o
	$(CC) $(CFLAGS) bin/dict.test.o bin/str.o bin/test_utils.o bin/memory.o bin/dict_extended.o bin/item.o bin/utils.o bin/dict.o bin/list.o -o test/dict

test/heap: bin/heap.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/heap.o bin/item.o bin/utils.o bin/dict.o bin/linked_list.o bin/list.o
	$(CC) $(CFLAGS) bin/heap.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/heap.o bin/item.o bin/utils.o bin/dict.o bin/linked_list.o bin/list.o -o test/heap

test/set: bin/set.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/dict_extended.o bin/set_extended.o bin/item.o bin/utils.o bin/set.o bin/dict.o bin/list.o
	$(CC) $(CFLAGS) bin/set.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/dict_extended.o bin/set_extended.o bin/item.o bin/utils.o bin/set.o bin/dict.o bin/list.o -o test/set

test/memory: bin/memory.test.o bin/memory.o bin/utils.o
	$(CC) $(CFLAGS) bin/memory.test.o bin/memory.o bin/utils.o -o test/memory

test/dict_perf: bin/dict_perf.test.o bin/str.o bin/test_utils.o bin/memory.o bin/item.o bin/utils.o bin/dict.o bin/list.o
	$(CC) $(CFLAGS) bin/dict_perf.test.o bin/str.o bin/test_utils.o bin/memory.o bin/item.o bin/utils.o bin/dict.o bin/list.o -o test/dict_perf

test/list_perf: bin/list_perf.test.o bin/str.o bin/test_utils.o bin/memory.o bin/utils.o bin/list.o
	$(CC) $(CFLAGS) bin/list_perf.test.o bin/str.o bin/test_utils.o bin/memory.o bin/utils.o bin/list.o -o test/list_perf

test/linked_list: bin/linked_list.test.o bin/str.o bin/test_utils.o bin/memory.o bin/utils.o bin/linked_list.o bin/list.o
	$(CC) $(CFLAGS) bin/linked_list.test.o bin/str.o bin/test_utils.o bin/memory.o bin/utils.o bin/linked_list.o bin/list.o -o test/linked_list

test/heap_perf: bin/heap_perf.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/heap.o bin/utils.o bin/item.o bin/dict.o bin/linked_list.o bin/list.o
	$(CC) $(CFLAGS) bin/heap_perf.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/heap.o bin/utils.o bin/item.o bin/dict.o bin/linked_list.o bin/list.o -o test/heap_perf

test/utils: bin/utils.test.o bin/utils.o
	$(CC) $(CFLAGS) bin/utils.test.o bin/utils.o -o test/utils

test/concurrency: bin/concurrency.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/dict_extended.o bin/set_conn.o bin/heap.o bin/dict_conn.o bin/utils.o bin/item.o bin/set.o bin/dict.o bin/heap_conn.o bin/list_conn.o bin/linked_list.o bin/list.o
	$(CC) $(CFLAGS) bin/concurrency.test.o bin/str.o bin/list_extended.o bin/test_utils.o bin/memory.o bin/dict_extended.o bin/set_conn.o bin/heap.o bin/dict_conn.o bin/utils.o bin/item.o bin/set.o bin/dict.o bin/heap_conn.o bin/list_conn.o bin/linked_list.o bin/list.o -o test/concurrency

bin/dict.o: src/dict.c
	$(CC) -o bin/dict.o -c src/dict.c

bin/list.o: src/list.c
	$(CC) -o bin/list.o -c src/list.c

bin/list.test.o: src/list.test.c
	$(CC) -o bin/list.test.o -c src/list.test.c

bin/str.o: src/str.c
	$(CC) -o bin/str.o -c src/str.c

bin/str.test.o: src/str.test.c
	$(CC) -o bin/str.test.o -c src/str.test.c

bin/dict.test.o: src/dict.test.c
	$(CC) -o bin/dict.test.o -c src/dict.test.c

bin/set.o: src/set.c
	$(CC) -o bin/set.o -c src/set.c

bin/heap.test.o: src/heap.test.c
	$(CC) -o bin/heap.test.o -c src/heap.test.c

bin/memory.o: src/memory.c
	$(CC) -o bin/memory.o -c src/memory.c

bin/set.test.o: src/set.test.c
	$(CC) -o bin/set.test.o -c src/set.test.c

bin/item.o: src/item.c
	$(CC) -o bin/item.o -c src/item.c

bin/test_utils.o: src/test_utils.c
	$(CC) -o bin/test_utils.o -c src/test_utils.c

bin/memory.test.o: src/memory.test.c
	$(CC) -o bin/memory.test.o -c src/memory.test.c

bin/dict_conn.o: src/dict_conn.c
	$(CC) -o bin/dict_conn.o -c src/dict_conn.c

bin/dict_perf.test.o: src/dict_perf.test.c
	$(CC) -o bin/dict_perf.test.o -c src/dict_perf.test.c

bin/list_perf.test.o: src/list_perf.test.c
	$(CC) -o bin/list_perf.test.o -c src/list_perf.test.c

bin/list_conn.o: src/list_conn.c
	$(CC) -o bin/list_conn.o -c src/list_conn.c

bin/linked_list.test.o: src/linked_list.test.c
	$(CC) -o bin/linked_list.test.o -c src/linked_list.test.c

bin/list_extended.o: src/list_extended.c
	$(CC) -o bin/list_extended.o -c src/list_extended.c

bin/dict_extended.o: src/dict_extended.c
	$(CC) -o bin/dict_extended.o -c src/dict_extended.c

bin/set_extended.o: src/set_extended.c
	$(CC) -o bin/set_extended.o -c src/set_extended.c

bin/heap_conn.o: src/heap_conn.c
	$(CC) -o bin/heap_conn.o -c src/heap_conn.c

bin/utils.o: src/utils.c
	$(CC) -o bin/utils.o -c src/utils.c

bin/set_conn.o: src/set_conn.c
	$(CC) -o bin/set_conn.o -c src/set_conn.c

bin/linked_list.o: src/linked_list.c
	$(CC) -o bin/linked_list.o -c src/linked_list.c

bin/heap.o: src/heap.c
	$(CC) -o bin/heap.o -c src/heap.c

bin/heap_perf.test.o: src/heap_perf.test.c
	$(CC) -o bin/heap_perf.test.o -c src/heap_perf.test.c

bin/utils.test.o: src/utils.test.c
	$(CC) -o bin/utils.test.o -c src/utils.test.c

bin/concurrency.test.o: src/concurrency.test.c
	$(CC) -o bin/concurrency.test.o -c src/concurrency.test.c


clean:
	rm -rf bin/* test/*
