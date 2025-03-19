#include "environment.h"

#include <stdlib.h>
#include <pthread.h>

priority_queues_t* pqueues;
task_queue_t* waiting_queue;
resource_queue_t* resources;
pthread_mutex_t lock;


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