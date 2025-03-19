#include "environment.h"
#include "scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



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
