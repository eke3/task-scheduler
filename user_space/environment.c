#include "environment.h"

#include <pthread.h>
#include <stdio.h>

priority_queues_t* task_queues;

task_queue_t* waiting_tasks;
task_queue_t* completed_tasks;

resource_queue_t* resources;

int set_up(void) {
    if ((task_queues = create_priority_queues()) == NULL) {
        return FAILURE;
    }
    if ((waiting_tasks = create_task_queue()) == NULL) {
        return FAILURE;
    }
    if ((completed_tasks = create_task_queue()) == NULL) {
        return FAILURE;
    }
    if ((resources = create_resource_queue()) == NULL) {
        return FAILURE;
    }
    return 0;
}

int tear_down(void) {
    free_task_queue(waiting_tasks);
    free_task_queue(completed_tasks);
    free_resource_queue(resources);

    if (free_priority_queues(task_queues) == FAILURE) {
        return FAILURE;
    }
    return 0;
}


int has_pending_tasks(void) {
    if (task_queues == NULL || waiting_tasks == NULL) {
        return FAILURE;
    }
    if (task_queues->high_priority_tasks == NULL || task_queues->medium_priority_tasks == NULL || task_queues->low_priority_tasks == NULL) {
        return FAILURE;
    }

    if (has_task(task_queues->high_priority_tasks)) {
        return 1;
    }
    if (has_task(task_queues->medium_priority_tasks)) {
        return 1;
    }
    if (has_task(task_queues->low_priority_tasks)) {
        return 1;
    }
    if (has_task(waiting_tasks)) {
        return 1;
    }
    return 0;
}

int to_pqueue(task_t* task) {
    if (task_queues == NULL || task == NULL) {
        return FAILURE;
    }

    switch (task->priority) {
        case HIGH_PRIORITY:
            return enqueue_task(task_queues->high_priority_tasks, task);
        case MEDIUM_PRIORITY:
            return enqueue_task(task_queues->medium_priority_tasks, task);
        case LOW_PRIORITY:
            return enqueue_task(task_queues->low_priority_tasks, task);
    }

    return FAILURE;
}