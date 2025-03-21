// File:    scheduler.c
// Author:  Eric Ekey
// Date:    03/19/2025
// Desc:    This file contains functions for executing and scheduling tasks.

#include "scheduler.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_mutex_t lock;
extern resource_queue_t* resources;
extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;

void execute_task(task_t* task) {
    // Mimic task execution by sleeping for its duration.
    printf("Executing task %d...\n", task->tid);
    sleep(task->duration);
    printf("Task %d completed\n", task->tid);
}

void process_pqueue(task_queue_t* pqueue) {
    task_t* task;

    pthread_mutex_lock(&lock);
    while ((task = dequeue_task(pqueue)) != NULL) {
        if (can_acquire_resources(task)) {
            // Task can acquire resources, acquire them and execute it.
            acquire_resources(task);
            pthread_mutex_unlock(&lock);
            execute_task(task);
            pthread_mutex_lock(&lock);
            release_resources(task);
            free(task->resources);
            free(task);
        } else {
            // Task cannot acquire resources, add it to the waiting queue.
            enqueue_task(waiting_queue, task);
        }
    }
    pthread_mutex_unlock(&lock);
}

void process_waiting_queue() {
    if (waiting_queue->head == NULL) {
        // No tasks in the waiting queue.
        return;
    }

    int* ids = calloc(100, sizeof(int)); // IDs of waiting tasks that can now acquire resources.
    int count = 0; // Number of tasks that can acquire resources.

    pthread_mutex_lock(&lock);
    task_t* curr = waiting_queue->head;
    while (curr != NULL) {
        if (can_acquire_resources(curr)) {
            // Task can acquire resources. add its ID to the array.
            ids[count] = curr->tid;
            count++;
        }
        curr = curr->next;
    }

    // Add the waiting tasks that can acquire resources to appropriate priority queues.
    for (int i = 0; i < count; i++) {
        to_pqueues(remove_task(waiting_queue, ids[i]));
    }
    pthread_mutex_unlock(&lock); // Unlock the mutex.
    free(ids);
}

void schedule_tasks() {
    while (are_there_any_uncompleted_tasks_left()) {
        process_pqueue(pqueues->high);
        process_pqueue(pqueues->medium);
        process_pqueue(pqueues->low);
        process_waiting_queue();
    }
}

