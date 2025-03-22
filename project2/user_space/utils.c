// File:    utils.c
// Author:  Eric Ekey
// Date:    03/19/2025
// Desc:    This file contains utility functions for the scheduler environment.

#include "utils.h"

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

extern resource_queue_t* resources;
extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;

void acquire_resources(task_t* task) {
    // printf("acquiring resources for task %d\n", task->tid);
    // if (can_acquire_resources(task)) {
        if (task->resources == NULL) {
            // No resources were needed to run this task.
            return;
        }
        
        for (int i = 0; i < task->num_resources*2; i=i+2) {
            printf("i: %d\n", i);
            // Get the ID and quantity of the resource.
            int rid = task->resources[i];
            int num_needed = task->resources[i+1];
            // sem_getvalue(task->resources[i].sem, &num_needed);

            // Find the resource in the resource queue using its ID and decrement its resource count.
            resource_t* resource = find_resource_id(resources, rid);
            for (int j = 0; j < num_needed; j++) {
                sem_wait(resource->sem);
            }
        }
        // printf("acquired resources for task %d\n", task->tid);
    // }
}

bool are_there_any_uncompleted_tasks_left() {
    // Check if there are any tasks in the priority queues.
    if (pqueues) {
        if (pqueues->high) {
            if (pqueues->high->head != NULL) {
                return true;
            }
        }
        if (pqueues->medium) {
            if (pqueues->medium->head != NULL) {
                return true;
            }
        }
        if (pqueues->low) {
            if (pqueues->low->head != NULL) {
                return true;
            }
        }
    }
    return false;
}

bool are_there_any_waiting_tasks_left() {
    // Check if there are any tasks in the waiting queue.
    if (waiting_queue) {
        if (waiting_queue->head != NULL) {
            return true;
        }
    }
    return false;
}


bool can_acquire_resources(task_t* task) {
    bool can_acquire = false;

    if (task) {
        can_acquire = true;
        if (task->resources == NULL) {
            // No resources needed.
            return true;
        }

        for (int i = 0; i < task->num_resources*2; i=i+2) {
            // Get the ID and quantity of the resource.
            int rid = task->resources[i];
            int num_needed = task->resources[i+1];
            // printf("rid: %d, num_needed: %d\n", rid, num_needed);
            // printf("pre-seg1\n");
            // printf("is null: %d\n", task->resources[i].sem == NULL);
            // sem_getvalue(task->resources[i].sem, &num_needed);
            // printf("post-seg1\n");
            // Find the resource in the resource queue using its ID and get its quantity.
            resource_t* resource;
            int num_available = 0;
            if ((resource = find_resource_id(resources, rid)) != NULL) {
                // printf("pre-seg2\n");
                sem_getvalue(resource->sem, &num_available);
                // printf("num_available: %d\n", num_available);
                // printf("post-seg2\n");
            }

            // Check if enough of the resource is available.
            if (num_available < num_needed) {
                can_acquire = false;
                break;
            }
            // printf("can acquire for task %d: %d\n", task->tid, can_acquire);
        }
    }
    return can_acquire;
}

void print_pqueues(priority_queues_t* pqueues) {
    if (pqueues) {
        printf("Printing priority queues...\n");
        printf("High:\n");
        print_tqueue(pqueues->high);
        printf("Medium:\n");
        print_tqueue(pqueues->medium);
        printf("Low:\n");
        print_tqueue(pqueues->low);
    }
}

void print_rqueue(resource_queue_t* rqueue) {
    if (rqueue){
        resource_t* curr = rqueue->head;
        printf("Printing resource queue...\n{\n");
        while (curr != NULL) {
            int quantity;
            sem_getvalue(curr->sem, &quantity);
            printf("\tResource ID: %d\tQuantity: %d\n", curr->rid, quantity);
            curr = curr->next;
        }
        printf("}\n");
    }
}

void print_tqueue(task_queue_t* tqueue) {
    if (tqueue) {
        task_t* curr = tqueue->head;
        printf("{\n");
        while (curr != NULL) {
            printf("\tTask ID: %d\n", curr->tid);
            curr = curr->next;
        }
        printf("}\n");
    }
}

void release_resources(task_t* task) {
    // printf("releasing resources for task %d\n", task->tid);
    if (task) {
        if (task->resources == NULL) {
            // No resources were needed to run this task.
            return;
        }

        for (int i = 0; i < task->num_resources*2; i=i+2) {
            // Get the ID and quantity of the resource.
            int rid = task->resources[i];
            int num_needed = task->resources[i+1];
            // sem_getvalue(task->resources[i].sem, &num_needed);

            // Find the resource in the resource queue using its ID and increment its resource count.
            resource_t* resource = find_resource_id(resources, rid);
            for (int j = 0; j < num_needed; j++) {
                sem_post(resource->sem);
            }
        }
    }
    // printf("released resources for task %d\n", task->tid);
}

void to_pqueues(task_t* task) {
    if (task) {
        switch (task->priority) {
            case HIGH:
                enqueue_task(pqueues->high, task);
                break;
            case MEDIUM:
                enqueue_task(pqueues->medium, task);
                break;
            case LOW:
                enqueue_task(pqueues->low, task);
                break;
        }
    }
}
