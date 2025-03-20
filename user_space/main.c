// File:    main.c
// Author:  Eric Ekey
// Date:    03/19/2025
// Desc:    This file contains the main function for simulating the scheduler environment.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "environment.h"
#include "scheduler.h"

extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;
extern resource_queue_t* resources;
extern pthread_mutex_t lock;

int main() {
    set_up();

    schedule_tasks();

    tear_down();

    return 0;
}
