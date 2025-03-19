#include "scheduler.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;
extern resource_queue_t* resources;
extern pthread_mutex_t lock;

void execute_task(task_t* task) {
    sleep(task->duration);
}

void schedule_tasks() {
    while (are_there_any_uncompleted_tasks_left()) {
        process_pqueue(pqueues->high);
        process_pqueue(pqueues->medium);
        process_pqueue(pqueues->low);
        process_waiting_queue();
    }
}

void process_pqueue(task_queue_t* pqueue) {
    task_t* task;

    pthread_mutex_lock(&lock); // Lock the mutex.
    while ((task = dequeue_task(pqueue)) != NULL) {
        if (can_acquire_resources(task)) {
            acquire_resources(task);
            pthread_mutex_unlock(&lock); // Unlock the mutex.
            execute_task(task);
            pthread_mutex_lock(&lock); // Lock the mutex.
            release_resources(task);
            free(task->resources);
            free(task);
        } else {
            enqueue_task(waiting_queue, task);
        }
    }
    pthread_mutex_unlock(&lock); // Unlock the mutex.
}

void process_waiting_queue() {
    if (waiting_queue->head == NULL) {
        // No tasks in the waiting queue.
        return;
    }

    int* ids = calloc(100, sizeof(int)); // ids of waiting tasks that can now acquire resources
    int count = 0; // number of tasks that can acquire resources.

    pthread_mutex_lock(&lock); // Lock the mutex.
    task_t* curr = waiting_queue->head;
    while (curr != NULL) {
        if (can_acquire_resources(curr)) {
            ids[count] = curr->tid; // Add the task id to the array if it can acquire resources.
            count++;
        }
        curr = curr->next;
    }

    for (int i = 0; i < count; i++) {
        to_pqueues(remove_task(waiting_queue, ids[i]));
    }
    pthread_mutex_unlock(&lock); // Unlock the mutex.
    free(ids);
}