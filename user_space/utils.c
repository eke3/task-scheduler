#include "utils.h"

#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;
extern task_queue_t* completed_queue;
extern resource_queue_t* resources;

bool are_there_any_uncompleted_tasks_left() {
    // Check if there are any tasks in the waiting queue.
    if (waiting_queue) {
        if (waiting_queue->head != NULL) {
            return true;
        }
    }

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

void print_tqueue(task_queue_t* tqueue) {
    if (tqueue == NULL) {
        printf("no task queue to print");
    } else {
        task_t* curr = tqueue->head;
        printf("{\n");
        while (curr != NULL) {
            printf("Task ID: %d\n", curr->tid);
            curr = curr->next;
        }
        printf("}\n");
    }
}

void print_rqueue(resource_queue_t* rqueue) {
    if (rqueue == NULL) {
        printf("no resource queue to print");
    } else {
        resource_t* curr = rqueue->head;
        printf("{\n");
        while (curr != NULL) {
            printf("Resource ID: %d\n", curr->rid);
            curr = curr->next;
        }
        printf("}\n");
    }
}

void print_pqueues(priority_queues_t* pqueues) {
    if (pqueues == NULL) {
        printf("no priority queues to print");
    } else {
        printf("Priority Queues:\n");
        printf("High:\n");
        print_tqueue(pqueues->high);
        printf("Medium:\n");
        print_tqueue(pqueues->medium);
        printf("Low:\n");
        print_tqueue(pqueues->low);
    }
}

bool can_acquire_resources(task_t* task) {
    bool can_acquire = true;

    if (task) {
        if (task->resources == NULL) {
            // No resources needed
            return true;
        }

        for (int i = 0; task->resources[i].rid > 0; i++) {
            // get the id and quantity of the resource
            int rid = task->resources[i].rid;
            int num_needed;
            sem_getvalue(task->resources[i].sem, &num_needed);

            // find the resource in the resource queue
            // get its quantity
            resource_t* resource;
            int num_available = 0;
            if ((resource = find_resource_id(resources, rid)) != NULL) {
                sem_getvalue(resource->sem, &num_available);
            }

            if (num_available < num_needed) {
                can_acquire = false;
                break;
            }
        }
    }
    return can_acquire;
}

void acquire_resources(task_t* task) {
    if (can_acquire_resources(task)) {
        for (int i = 0; task->resources[i].rid > 0; i++) {
            // get the id and quantity of the resource
            int rid = task->resources[i].rid;
            int num_needed;
            sem_getvalue(task->resources[i].sem, &num_needed);

            // find the resource in the resource queue
            // decrement its resource count
            resource_t* resource = find_resource_id(resources, rid);
            for (int j = 0; j < num_needed; j++) {
                sem_wait(resource->sem);
            }
        }
    }
}

void release_resources(task_t* task) {
    if (task) {
        if (task->resources == NULL) {
            // No resources were needed to run this task.
            return;
        }

        for (int i = 0; task->resources[i].rid > 0; i++) {
            // get the id and quantity of the resource
            int rid = task->resources[i].rid;
            int num_needed;
            sem_getvalue(task->resources[i].sem, &num_needed);

            // find the resource in the resource queue
            // increment its resource count
            resource_t* resource = find_resource_id(resources, rid);
            for (int j = 0; j < num_needed; j++) {
                sem_post(resource->sem);
            }
        }
    }
}