#include "environment.h"
#include "scheduler.h"

#include <stdio.h>
#include <stdlib.h>

extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;
extern task_queue_t* completed_queue;
extern resource_queue_t* resources;

void test_adding_duplicate_resource();
void test_adding_duplicate_task();
void test_acquire_resources();

int main() {
    printf("Hello world!\n");

    // set_up();

    test_adding_duplicate_resource();
    test_adding_duplicate_task();
    test_acquire_resources();
    // can populate queues here


    // schedule_tasks();

    // tear_down();

    return 0;
}

// Working
void test_adding_duplicate_resource() {
    set_up();

    // can populate queues here
    resource_t* resource1 = create_resource(1, create_semaphore(1));
    resource_t* resource2 = create_resource(2, create_semaphore(2));
    resource_t* resource3 = create_resource(3, create_semaphore(3));
    resource_t* resource4 = create_resource(1, create_semaphore(4)); // Dupe of resource1 should be added to resource 1 quantity

    enqueue_resource(resources, resource1);
    enqueue_resource(resources, resource2);
    enqueue_resource(resources, resource3);
    enqueue_resource(resources, resource4);

    print_rqueue(resources);

    tear_down();
}

// Working
void test_adding_duplicate_task() {
    set_up();

    sem_t sem;
    sem_init(&sem, 0, 1);
    resource_t* resource = calloc(2, sizeof(resource_t));
    resource_t rsrc;
    rsrc.rid = 1;
    rsrc.sem = &sem;
    rsrc.next = NULL;
    resource[0] = rsrc;
    resource_t rsrc2;
    rsrc2.rid = 0;
    rsrc2.sem = NULL;
    rsrc2.next = NULL;
    resource[1] = rsrc2;

    task_t* task1 = create_task(1, HIGH, 1, resource);
    task_t* task2 = create_task(2, HIGH, 1, NULL);
    task_t* task3 = create_task(3, HIGH, 1, NULL);
    task_t* task4 = create_task(1, HIGH, 1, NULL); // Dupe of task1 should be ignored if put in the same queue

    enqueue_task(pqueues->high, task1);
    enqueue_task(pqueues->high, task2);
    enqueue_task(pqueues->high, task3);
    enqueue_task(pqueues->high, task4);

    print_tqueue(pqueues->high);

    tear_down();
}

// Working
void test_acquire_resources() {
    set_up();
    resource_t* resource1 = create_resource(1, create_semaphore(1));
    resource_t* resource2 = create_resource(2, create_semaphore(2));
    resource_t* resource3 = create_resource(3, create_semaphore(3));
    enqueue_resource(resources, resource1);
    enqueue_resource(resources, resource2);
    enqueue_resource(resources, resource3);
    print_rqueue(resources);

    sem_t sem;
    sem_init(&sem, 0, 2);
    resource_t* resource = calloc(2, sizeof(resource_t));
    resource_t rsrc;
    rsrc.rid = 3;
    rsrc.sem = &sem;
    rsrc.next = NULL;
    resource[0] = rsrc;
    task_t* task1 = create_task(1, HIGH, 1, resource); // this task will need one of resource 1
    enqueue_task(pqueues->high, task1);

    acquire_resources(task1);
    print_rqueue(resources);
    release_resources(task1);
    print_rqueue(resources);

    tear_down();
}