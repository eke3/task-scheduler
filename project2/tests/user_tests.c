#include "../user_space/environment.h"
#include "../user_space/scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;
extern resource_queue_t* resources;
pthread_mutex_t test_pqueues_lock;

void test_acquire_release_resources();
void test_schedule_tasks();
void test_synchronization();
void* create_tasks(void* arg);

int main() {
    test_acquire_release_resources();
    test_schedule_tasks();
    test_synchronization();
    return 0;
}

// Working
void test_acquire_release_resources() {
    set_up();
    resource_t* resource1 = create_resource(1, create_semaphore(1));
    resource_t* resource2 = create_resource(2, create_semaphore(2));
    resource_t* resource3 = create_resource(3, create_semaphore(3));
    enqueue_resource(resources, resource1);
    enqueue_resource(resources, resource2);
    enqueue_resource(resources, resource3);
    print_rqueue(resources);

    int* resource = calloc(2, sizeof(int));
    resource[0] = 1;
    resource[1] = 1;
    task_t* task1 = create_task(1, HIGH, 1, resource, 1); // this task will need one of resource 1
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
    int* resource = malloc(6 * sizeof(int));
    for (int i = 0; i < 3*2; i=i+2) {
        resource[i] = i;
        resource[i+1] = 1;
        task_t* task = create_task(1, HIGH, 1, resource, 3);
        to_pqueues(task);
    }

    // tear_down();
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

    task_t* waiting_task = create_task(100, LOW, 1, NULL, 0); // Create a new task
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
        task_t* task = create_task(i, ((i % 2 == 0) ? HIGH : MEDIUM), 1, NULL, 0);
        printf("Adding task %d to queue\n", i);
        pthread_mutex_lock(&test_pqueues_lock);
        to_pqueues(task);
        pthread_mutex_unlock(&test_pqueues_lock);
    }
    return NULL;
}
