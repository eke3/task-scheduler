// File:    environment.c
// Author:  Eric Ekey
// Date:    03/19/2025
// Desc:    This file contains global variables and functions for initializing and tearing down the scheduler environment.

#include "environment.h"

#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock; // Mutex for synchronization.
resource_queue_t* resources; // Queue for existing resources.
priority_queues_t* pqueues; // Priority queues (HIGH, MEDIUM, LOW).
task_queue_t* waiting_queue; // Queue for waiting tasks.

void set_up() {
    pqueues = create_priority_queues();
    waiting_queue = create_task_queue();
    resources = create_resource_queue();
    pthread_mutex_init(&lock, NULL);

    if (pqueues == NULL || waiting_queue == NULL || resources == NULL) {
        tear_down();
        exit(EXIT_FAILURE);
    }
}

void tear_down() {
    free_priority_queues(pqueues);
    free_task_queue(waiting_queue);
    free_resource_queue(resources);
    pthread_mutex_destroy(&lock);
}
