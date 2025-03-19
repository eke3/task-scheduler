CC = gcc
CFLAGS = -g -Wall -O0 -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm
VALGRIND_FLAGS = --leak-check=full
EXTRA_VALGRIND_FLAGS = --show-leak-kinds=all --track-origins=yes -s

TARGET = task_scheduler
SOURCES = main.c environment.c utils.c queue.c task.c resource.c
OBJECTS = $(SOURCES:.c=.o)

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	rm -f $(OBJECTS)

main.o: main.c environment.o
	$(CC) $(CFLAGS) -c main.c $(LDFLAGS)

# scheduler.o: scheduler.h scheduler.c environment.o
# 	$(CC) $(CFLAGS) -c scheduler.c $(LDFLAGS)

environment.o: environment.h environment.c utils.o
	$(CC) $(CFLAGS) -c environment.c $(LDFLAGS)

utils.o: utils.h utils.c queue.o
	$(CC) $(CFLAGS) -c utils.c $(LDFLAGS)

queue.o: queue.h queue.c task.o
	$(CC) $(CFLAGS) -c queue.c $(LDFLAGS)

task.o: task.h task.c resource.o
	$(CC) $(CFLAGS) -c task.c $(LDFLAGS)

resource.o: resource.h resource.c
	$(CC) $(CFLAGS) -c resource.c $(LDFLAGS)

.PHONY: run val val_extra clean

run:
	./$(TARGET)

val:
	valgrind $(VALGRIND_FLAGS) ./$(TARGET)

val_extra:
	valgrind $(VALGRIND_FLAGS) $(EXTRA_VALGRIND_FLAGS) ./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS) core