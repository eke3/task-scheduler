#include "scheduler.h"

#include <pthread.h>

extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;
extern task_queue_t* completed_queue;
extern resource_queue_t* resources;

pthread_mutex_t lock;

void execute_task(task_t* task) {
    printf("Executing task %d\n", task->tid);
    sleep(task->duration);
    printf("Task %d completed\n", task->tid);
}

void schedule_tasks() {
    while (are_there_any_uncompleted_tasks_left()) {
        process_pqueue(pqueues->high);
        process_pqueue(pqueues->medium);
        process_pqueue(pqueues->low);
        process_waiting_queue();
    }
}
// looking at high pqueue:
// freeze mutex
// dequeue first task from high pqueue
// acquire task resources (modifies rqueue)
// if no resources, enqueue task to wait queue
// if resources, execute
// after execution, release task resources
// enqueue executed task to completed queue
// unfreeze mutex

// looking at medium pqueue
// do the same

// looking at low pqueue
// do the same

void process_pqueue(task_queue_t* pqueue) {
    task_t* task;

    pthread_mutex_init(&lock, NULL); // Lock the mutex.
    while ((task = dequeue_task(pqueue)) != NULL) {
        if (can_acquire_resources(task)) {
            acquire_resources(task);
            execute_task(task);
            release_resources(task);
            enqueue_task(completed_queue, task);
        } else {
            enqueue_task(waiting_queue, task);
        }
    }
    pthread_mutex_destroy(&lock); // Unlock the mutex.
}

void process_waiting_queue() {
    task_t* task;

    pthread_mutex_init(&lock, NULL); // Lock the mutex.
    while ((task = dequeue_task(waiting_queue)) != NULL) {
        if (can_acquire_resources(task)) {
            to_pqueues(task);
        } else {
            enqueue_task(waiting_queue, task);
        }
    }
    pthread_mutex_destroy(&lock); // Unlock the mutex.
}