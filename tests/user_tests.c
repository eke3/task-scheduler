#include "../user_space/environment.h"
#include "../user_space/scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;
extern resource_queue_t* resources;
extern pthread_mutex_t lock;

void test_adding_duplicate_resource();
void test_adding_duplicate_task();
void test_acquire_resources();
void test_schedule_tasks();
void test_synchronization();
void* create_tasks(void* arg);

int main() {
    // set_up();

    // test_adding_duplicate_resource();
    // test_adding_duplicate_task();
    // test_acquire_resources();
    // test_schedule_tasks();
    // test_synchronization();


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

// Working
void test_schedule_tasks() {
    set_up();
    // Resource Queue
    resource_t* resource1 = create_resource(1, create_semaphore(1));
    resource_t* resource2 = create_resource(2, create_semaphore(2));
    resource_t* resource3 = create_resource(3, create_semaphore(3));
    enqueue_resource(resources, resource1);
    enqueue_resource(resources, resource2);
    enqueue_resource(resources, resource3);
    print_rqueue(resources);

    // enqueue some tasks
    sem_t sem;
    sem_init(&sem, 0, 1);
    resource_t* resource = malloc(2 * sizeof(resource_t));
    for (int i = 0; i < 2; i++) {
        resource_t rsr;
        rsr.rid = 0;
        resource[i] = rsr;
    }
    resource_t rsrc;
    rsrc.rid = 1;
    rsrc.sem = &sem;
    rsrc.next = NULL;
    resource[0] = rsrc;
    task_t* task1 = create_task(1, HIGH, 1, resource); // this task will need one of resource 1
    to_pqueues(task1);

    sem_t sem2;
    sem_init(&sem2, 0, 2);
    resource_t* resources2 = malloc(2 * sizeof(resource_t));
    for (int i = 0; i < 2; i++) {
        resource_t rsr;
        rsr.rid = 0;
        resources2[i] = rsr;
    }
    resource_t rsrc2;
    rsrc2.rid = 3;
    rsrc2.sem = &sem2;
    rsrc2.next = NULL;
    resources2[0] = rsrc2;
    task_t* task2 = create_task(2, HIGH, 1, resources2); // this task will need two of resource 3
    to_pqueues(task2);

    sem_t sem3;
    sem_init(&sem3, 0, 9);
    resource_t* resources3 = malloc(2 * sizeof(resource_t));
    for (int i = 0; i < 2; i++) {
        resource_t rsr;
        rsr.rid = 0;
        resources3[i] = rsr;
    }
    resource_t rsrc3;
    rsrc3.rid = 2;
    rsrc3.sem = &sem3;
    rsrc3.next = NULL;
    resources3[0] = rsrc3;
    task_t* task3 = create_task(3, HIGH, 1, resources3); // this task will need one of resource 2
    to_pqueues(task3);

    // schedule_tasks();

    tear_down();
}

// Working
void test_synchronization() {
    set_up();

    pthread_t t1, t2, t3;
    int start1 = 1, start2 = 6, start3 = 11;
    pthread_create(&t1, NULL, create_tasks, &start1);
    pthread_create(&t2, NULL, create_tasks, &start2);
    pthread_create(&t3, NULL, create_tasks, &start3);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    task_t* waiting_task = create_task(100, LOW, 1, NULL); // Create a new task
    enqueue_task(waiting_queue, waiting_task); // Enqueue the task into the waiting queue


    print_pqueues(pqueues);
    schedule_tasks();
    print_pqueues(pqueues);
    tear_down();
}

void* create_tasks(void* arg) {
    int start_id = *((int*)arg);
    for (int i = start_id; i < start_id + 2; i++) {
        printf("Creating task %d\n", i);
        task_t* task = create_task(i, ((i % 2 == 0) ? HIGH : MEDIUM), 1, NULL);
        pthread_mutex_lock(&lock);
        printf("Adding task %d to queue\n", i);
        to_pqueues(task);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
