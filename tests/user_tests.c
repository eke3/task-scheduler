#include <stdlib.h>
#include <stdio.h>

#include "environment.h"
#include "scheduler.h"

priority_queues_t* task_queues;

task_queue_t* waiting_tasks;
task_queue_t* completed_tasks;

resource_queue_t* resources;



int scene_1(void);
int scene_2(void);
int scene_3(void);

int main() {

    if (scene_1() == FAILURE) {
        fprintf(stderr, "scene 1 failed\n");
        return EXIT_FAILURE;
    }

    if (scene_2() == FAILURE) {
        fprintf(stderr, "scene 2 failed\n");
        return EXIT_FAILURE;
    }

    if (scene_3() == FAILURE) {
        fprintf(stderr, "scene 3 failed\n");
        return EXIT_FAILURE;
    }

    return 0;
}


int scene_1(void) {
    // set up
    if (set_up() == FAILURE) {
        tear_down();
        return FAILURE;
    }

    // Set up the scene, populate tasks and resources



    // run scheduler
    if (run_scheduler() == FAILURE) {
        tear_down();
        return FAILURE;
    }

    // tear down
    return tear_down();
}


int scene_2(void) {
    // set up
    if (set_up() == FAILURE) {
        tear_down();
        return FAILURE;
    }

    // Set up the scene, populate tasks and resources



    // run scheduler
    if (run_scheduler() == FAILURE) {
        tear_down();
        return FAILURE;
    }

    // tear down
    return tear_down();
}

int scene_3(void) {
    // set up
    if (set_up() == FAILURE) {
        tear_down();
        return FAILURE;
    }

    // Set up the scene, populate tasks and resources



    // run scheduler
    if (run_scheduler() == FAILURE) {
        tear_down();
        return FAILURE;
    }

    // tear down
    return tear_down();
}






