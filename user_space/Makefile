CC = gcc
CFLAGS = -g -Wall -O0 -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm
VALGRIND_FLAGS = --leak-check=full
EXTRA_VALGRIND_FLAGS = --show-leak-kinds=all --track-origins=yes -s

TARGET = task_scheduler
TEST_TARGET = ../tests/user_tests
SOURCES = main.c environment.c utils.c queue.c task.c resource.c scheduler.c
OBJECTS = $(SOURCES:.c=.o)

all: $(OBJECTS) user_tests.o
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	$(CC) $(CFLAGS) environment.o utils.o queue.o task.o resource.o scheduler.o user_tests.o -o $(TEST_TARGET) $(LDFLAGS)
	rm -f *.o

user_tests.o: ../tests/user_tests.c environment.o scheduler.o
	$(CC) $(CFLAGS) -c ../tests/user_tests.c $(LDFLAGS)

main.o: main.c environment.o scheduler.o
	$(CC) $(CFLAGS) -c main.c $(LDFLAGS)

scheduler.o: scheduler.h scheduler.c utils.o
	$(CC) $(CFLAGS) -c scheduler.c $(LDFLAGS)

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

.PHONY: run test val val_test val_extra clean

run:
	./$(TARGET)

test:
	./$(TEST_TARGET)

val_test:
	valgrind $(VALGRIND_FLAGS) ./$(TEST_TARGET)

val:
	valgrind $(VALGRIND_FLAGS) ./$(TARGET)

val_extra:
	valgrind $(VALGRIND_FLAGS) $(EXTRA_VALGRIND_FLAGS) ./$(TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o core