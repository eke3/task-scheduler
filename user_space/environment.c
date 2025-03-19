#include "environment.h"

#include <stdlib.h>

priority_queues_t* pqueues;
task_queue_t* waiting_queue;
task_queue_t* completed_queue;
resource_queue_t* resources;

void set_up() {
    pqueues = create_priority_queues();
    waiting_queue = create_task_queue();
    completed_queue = create_task_queue();
    resources = create_resource_queue();

    if (pqueues == NULL || waiting_queue == NULL || completed_queue == NULL || resources == NULL) {
        tear_down();
        exit(EXIT_FAILURE);
    }
}

void tear_down() {
    free_priority_queues(pqueues);
    // free_task_queue(pqueues->high);
    // free_task_queue(pqueues->medium);
    // free_task_queue(pqueues->low);
    // free(pqueues);
    free_task_queue(waiting_queue);
    free_task_queue(completed_queue);
    free_resource_queue(resources);
}