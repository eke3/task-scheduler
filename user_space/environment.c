#include "environment.h"

#include <stdio.h>

int set_up() {
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



    // if this is a test, populate task_queues and resources.



    return 0;
}

int tear_down() {
    if (free_priority_queues(task_queues) == FAILURE) {
        return FAILURE;
    }
    free_task_queue(waiting_tasks);
    free_task_queue(completed_tasks);
    free_resource_queue(resources);

    return 0;
}


int has_pending_tasks() {
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