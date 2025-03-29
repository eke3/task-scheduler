// File:    main.c
// Author:  Eric Ekey
// Date:    03/19/2025
// Desc:    This file contains the main function for simulating the scheduler environment.

#define NUM_RESOURCES 10 // Number of resources to make available to the scheduler.
#define NUM_TASKS 10 // Number of tasks to generate and schedule.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "environment.h"
#include "scheduler.h"

extern pthread_mutex_t pqueues_lock;
extern pthread_mutex_t waiting_queue_lock;
extern pthread_mutex_t resources_lock;
extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;
extern resource_queue_t* resources;

pthread_t thread_pool[9]; // Pool of threads for generating and scheduling tasks.

// void add_resources()
// Description: Populates the resource queue in the scheduler environment.
// Preconditions: Environment is set up.
// Postconditions: Resource queue is populated with NUM_RESOURCES resources.
// Returns: None.
void add_resources(void);

// void* THREAD_generate_tasks(void* arg)
// Description: Creates tasks and injects them into the scheduler.
// Preconditions: Environment is set up.
// Postconditions: NUM_TASKS tasks are created and queued for scheduling.
// Returns: None.
void* THREAD_generate_tasks(void* arg);

// void* THREAD_schedule_tasks(void* arg)
// Description: Schedules tasks in the scheduler.
// Preconditions: Environment is set up.
// Postconditions: Queued tasks are scheduled and executed.
// Returns: None.
void* THREAD_schedule_tasks(void* arg);

int main() {
    set_up();
    add_resources();

    // Display all resources available to the scheduler.
    print_rqueue(resources);

    // Create 3 threads to generate tasks and 6 threads to schedule tasks.
    for (int i = 0, j = 3; i < 3; i++, j++) {
        pthread_create(&thread_pool[i], NULL, THREAD_generate_tasks, NULL);
        // pthread_create(&thread_pool[j], NULL, THREAD_schedule_tasks, NULL);
        // pthread_create(&thread_pool[j + 3], NULL, THREAD_schedule_tasks, NULL);
    }
sleep(1);
    for (int i = 0, j = 3; i < 3; i++, j++) {
        // pthread_create(&thread_pool[i], NULL, THREAD_generate_tasks, NULL);
        pthread_create(&thread_pool[j], NULL, THREAD_schedule_tasks, NULL);
        // pthread_create(&thread_pool[j + 3], NULL, THREAD_schedule_tasks, NULL);
    }

    // Wait for all threads to finish.
    for (int i = 0, j = 3; i < 3; i++, j++) {
        pthread_join(thread_pool[i], NULL);
        pthread_join(thread_pool[j], NULL);
        // pthread_join(thread_pool[j + 3], NULL);
    }

    // Resources and availability should match their initial values before scheduling tasks.
    print_rqueue(resources);
    tear_down();
    return EXIT_SUCCESS;
}


void add_resources() {
    // Make 10 resources and inject them into the scheduler.
    for (int i = 1; i <= NUM_RESOURCES; i++) {
        resource_t* new_resource = create_resource(i, create_semaphore(i));
        enqueue_resource(resources, new_resource);
    }
}

void* THREAD_generate_tasks(void* arg) {
    int min_rsrc = 0; // Minimum number of a resource needed to run a task.
    int max_rsrc = 5; // Maximum number of a resource needed to run a task.
    task_priority_t min_priority = LOW;
    task_priority_t max_priority = HIGH;

    // Create NUM_TASKS tasks and inject them into the scheduler.
    for (int i = 1, j = 0; i <= NUM_TASKS/3; i++, j++) {
        if (j > (NUM_RESOURCES-1)) {
            j = 0;
        }

        int task_id = rand();
        task_priority_t priority = (task_priority_t)(rand() % (max_priority - min_priority + 1)) + min_priority;
        int task_duration = 1;// (rand() % 3) + 1; // Task duration between 1 and 3 seconds.

        // Resources and quantities required for a task. ( [resource_id, quantity, resource_id, quantity, ...] )
        int* resource_array = malloc(4 * sizeof(int));
        resource_array[0] = (rand() % 9) + 1;
        resource_array[1] = (rand() % (max_rsrc - min_rsrc + 1)) + min_rsrc;
        resource_array[2] = resource_array[0] + 1; // Make sure the second resource is different from the first.
        resource_array[3] = (rand() % (max_rsrc - min_rsrc + 1)) + min_rsrc;

        task_t* task = create_task(task_id, priority, task_duration, resource_array, 2);
        pthread_mutex_lock(&pqueues_lock);
        if (task) to_pqueues(task);
        pthread_mutex_unlock(&pqueues_lock);
        printf("Task %d queued with %s priority\n", task_id, ((priority == HIGH) ? "HIGH" : (priority == MEDIUM) ? "MEDIUM" : "LOW"));
    }
    return NULL;
}

void* THREAD_schedule_tasks(void* arg) {
    schedule_tasks();
    return NULL;
}

