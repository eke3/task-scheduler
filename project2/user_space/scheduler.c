// File:    scheduler.c
// Author:  Eric Ekey
// Date:    03/19/2025
// Desc:    This file contains functions for executing and scheduling tasks.

#include "scheduler.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_mutex_t pqueues_lock;
extern pthread_mutex_t waiting_queue_lock;
extern pthread_mutex_t resources_lock;
extern resource_queue_t* resources;
extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;

void execute_task(task_t* task) {
    // Mimic task execution by sleeping for its duration.
    printf("Executing task %d...\n", task->tid);
    if (task) {
    sleep(task->duration);
    }
    printf("Task %d completed\n", task->tid);
}

void process_pqueue(task_queue_t* pqueue) {
    task_t* task;

    while ((task = dequeue_task(pqueue)) != NULL) {
        if (task->tid <= 0 || task->num_resources < 0) {
            
        } else {
            if (can_acquire_resources(task)) {
                // Task can acquire resources, acquire them and execute it.
                acquire_resources(task);
                execute_task(task);
                release_resources(task);
                // Add the task to the completed queue.
                if (task) {
                    if (task->resources) {
                    free(task->resources);
                }
                free(task);
                }
            } else {
                enqueue_task(waiting_queue, task);
            }
    }
    }
}

int process_waiting_queue() {
    bool tasks_are_old = true;

    if (waiting_queue->head == NULL) {
        // No tasks in the waiting queue.
        return tasks_are_old;
    }

    int* ids = calloc(100, sizeof(int)); // IDs of waiting tasks that can now acquire resources.
    int count = 0; // Number of tasks that can acquire resources.

    task_t* curr = waiting_queue->head;
    while (curr != NULL) {
        if (can_acquire_resources(curr)) {
            // Task can acquire resources. add its ID to the array.
            ids[count] = curr->tid;
            count++;
        }
        curr->age++;
        if (curr->age > 5) {
            // EXTRA CREDIT: Task has been waiting for 5 passes, increase priority.
            increase_priority(curr);
        }
        if (curr->age < TASK_OLD_AGE) {
            tasks_are_old = false;
        }
        curr = curr->next;
    }

    if (count == 0) {
        // No tasks can acquire resources, move on.
        free(ids);
        return tasks_are_old;
    }

    // Add the waiting tasks that can acquire resources to appropriate priority queues.
    pthread_mutex_lock(&pqueues_lock);
    for (int i = 0; i < count; i++) {
        task_t* task = remove_task(waiting_queue, ids[i]);
        task->age = 0;
        to_pqueues(task);
    }
    pthread_mutex_unlock(&pqueues_lock); // Unlock the mutex.
    free(ids);
    return tasks_are_old;
}

void schedule_tasks() {
    int stop_scheduler = 0;

    // Run the scheduler until there are no more tasks left or all tasks are old and can't acquire resources.
    while (are_there_any_uncompleted_tasks_left() || are_there_any_waiting_tasks_left()) {
        pthread_mutex_lock(&pqueues_lock);
        pthread_mutex_lock(&resources_lock);
        pthread_mutex_lock(&waiting_queue_lock);
        process_pqueue(pqueues->high);
        process_pqueue(pqueues->medium);
        process_pqueue(pqueues->low);
        pthread_mutex_unlock(&pqueues_lock);
        pthread_mutex_unlock(&waiting_queue_lock);
        
        pthread_mutex_lock(&waiting_queue_lock);
        stop_scheduler = process_waiting_queue();
        pthread_mutex_unlock(&resources_lock);

        if (!are_there_any_uncompleted_tasks_left() && stop_scheduler) {
            // The only remaining tasks are old and can't acquire resources, stop the scheduler.
            printf("All tasks are old and can't acquire resources, stopping scheduler\n");
            pthread_mutex_unlock(&waiting_queue_lock);
            break;
        }
        pthread_mutex_unlock(&waiting_queue_lock);
    }
}

