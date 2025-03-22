// File:    environment.c
// Author:  Eric Ekey
// Date:    03/19/2025
// Desc:    This file contains global variables and functions for initializing and tearing down the scheduler environment.

#include "environment.h"

#include <pthread.h>
#include <stdlib.h>

// pthread_mutex_t completed_queue_lock;
pthread_mutex_t pqueues_lock; // Mutex for synchronization.
pthread_mutex_t resources_lock; // Mutex for synchronization.
pthread_mutex_t waiting_queue_lock; // Mutex for synchronization.
resource_queue_t* resources; // Queue for existing resources.
priority_queues_t* pqueues; // Priority queues (HIGH, MEDIUM, LOW).
task_queue_t* waiting_queue; // Queue for waiting tasks.
// task_queue_t* completed_queue; // Queue for completed tasks.

void set_up() {
    pqueues = create_priority_queues();
    waiting_queue = create_task_queue();
    resources = create_resource_queue();
    // completed_queue = create_task_queue();
    pthread_mutex_init(&pqueues_lock, NULL);
    pthread_mutex_init(&resources_lock, NULL);
    pthread_mutex_init(&waiting_queue_lock, NULL);
    // pthread_mutex_init(&completed_queue_lock, NULL);

    if (pqueues == NULL || waiting_queue == NULL || resources == NULL/* || completed_queue == NULL */) {
        tear_down();
        exit(EXIT_FAILURE);
    }
}

void tear_down() {
    free_priority_queues(pqueues);
    free_task_queue(waiting_queue);
    free_resource_queue(resources);
    // free_task_queue(completed_queue);
    pthread_mutex_destroy(&pqueues_lock);
    pthread_mutex_destroy(&resources_lock);
    pthread_mutex_destroy(&waiting_queue_lock);
    // pthread_mutex_destroy(&completed_queue_lock);
}
