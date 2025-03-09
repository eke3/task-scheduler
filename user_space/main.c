#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "environment.h"

// priority_queues_t* task_queues;

// task_queue_t* waiting_tasks;
// task_queue_t* completed_tasks;

// resource_queue_t* resources;

int main() {
    // set up
    if (set_up() == FAILURE) {
        tear_down();
        return EXIT_FAILURE;
    }

    // run scheduler
    if (run_scheduler() == FAILURE) {
        tear_down();
        return EXIT_FAILURE;
    }

    // tear down
    // printf("tearing down: exit code %d\n", tear_down());
    // return 0;
    return tear_down();
}