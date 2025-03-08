CC = gcc
CFLAGS = -g -Wall -O0 -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm
VALGRIND_FLAGS = --leak-check=full
EXTRA_VALGRIND_FLAGS = --show-leak-kinds=all --track-origins=yes -s

TARGET = task_scheduler
SOURCES = main.c scheduler.c queue.c environment.c
OBJECTS = $(SOURCES:.c=.o)

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	rm -f $(OBJECTS)

main.o: main.c scheduler.o environment.o
	$(CC) $(CFLAGS) -c main.c $(LDFLAGS)

scheduler.o: scheduler.h scheduler.c environment.o
	$(CC) $(CFLAGS) -c scheduler.c $(LDFLAGS)

environment.o: environment.h environment.c queue.o
	$(CC) $(CFLAGS) -c environment.c $(LDFLAGS)

queue.o: queue.h queue.c
	$(CC) $(CFLAGS) -c queue.c $(LDFLAGS)

.PHONY: run val val_extra clean

run:
	./$(TARGET)

val:
	valgrind $(VALGRIND_FLAGS) ./$(TARGET)

val_extra:
	valgrind $(VALGRIND_FLAGS) $(EXTRA_VALGRIND_FLAGS) ./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS) core