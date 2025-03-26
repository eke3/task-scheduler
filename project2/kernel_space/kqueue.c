#include "kqueue.h"

#include <linux/slab.h>

priority_queues_t* create_priority_queues() {
    priority_queues_t* pqueues = NULL;
    if ((pqueues = (priority_queues_t*)kmalloc(sizeof(priority_queues_t), GFP_KERNEL)) == NULL) {
        return NULL;
    }
    if ((pqueues->high = create_task_queue()) == NULL) {
        return NULL;
    }
    if ((pqueues->medium = create_task_queue()) == NULL) {
        return NULL;
    }
    if ((pqueues->low = create_task_queue()) == NULL) {
        return NULL;
    }
    return pqueues;
}

resource_queue_t* create_resource_queue() {
    resource_queue_t* rqueue = NULL;
    if ((rqueue = (resource_queue_t*)kmalloc(sizeof(resource_queue_t), GFP_KERNEL)) != NULL) {
        rqueue->head = NULL;
        rqueue->tail = NULL;
    }
    return rqueue;
}

task_queue_t* create_task_queue() {
    task_queue_t* tqueue = NULL;
    if ((tqueue = (task_queue_t*)kmalloc(sizeof(task_queue_t), GFP_KERNEL)) != NULL) {
        tqueue->head = NULL;
        tqueue->tail = NULL;
    }
    return tqueue;
}

task_t* dequeue_task(task_queue_t* tqueue) {
    task_t* task = NULL;
    if (tqueue != NULL) {
        task = tqueue->head;
        if (task != NULL) {
            // Set the head to the next task.
            tqueue->head = task->next;
        }
    }
    return task;
}

void enqueue_resource(resource_queue_t* rqueue, resource_t* resource) {
    resource_t* existing_resource = NULL;
    int enqueue_quantity = 0;
    if ((rqueue != NULL) && (resource != NULL)) {
        if (rqueue->head == NULL) {
            rqueue->head = resource;
            rqueue->tail = resource;
        } else {
            rqueue->tail->next = resource;
            rqueue->tail = resource;
            resource->tail->next = NULL;
        }
    }
}

void enqueue_task(task_queue_t* tqueue, task_t* task) {
    if ((tqueue != NULL) && (task != NULL)) {
        if (tqueue->head == NULL) {
            tqueue->head = task;
            tqueue->tail = task;
        } else {
            tqueue->tail->next = task;
            tqueue->tail = task;
            task->tail->next = NULL;
        }
    }
}

void free_priority_queues(priority_queues_t* pqueues) {
    if (pqueues != NULL) {
        free_task_queue(pqueues->high);
        free_task_queue(pqueues->medium);
        free_task_queue(pqueues->low);
        kfree(pqueues);
    }
}

void free_resource_queue(resource_queue_t* rqueue) {
    resource_t* curr = NULL;
    if (rqueue != NULL) {
        while ((curr = dequeue_resource(rqueue)) != NULL) {
            kfree(curr);
        }
        kfree(rqueue);
    }
}

void free_task_queue(task_queue_t* tqueue) {
    task_t* curr = NULL;
    if (tqueue != NULL) {
        while ((curr = dequeue_task(tqueue)) != NULL) {
            kfree(curr);
        }
        kfree(tqueue);
    }
}

resource_t* find_resource_id(resource_queue_t* rqueue, int rid) {
    resource_t* curr = NULL;
    if (rqueue != NULL) {
        curr = rqueue->head;
        while (curr != NULL) {
            if (curr->rid == rid) {
                return curr;
            }
            curr = curr->next;
        }
    }
    return curr;
}

task_t* find_task_id(task_queue_t* tqueue, int tid) {
    task_t* curr = NULL;
    if (tqueue != NULL) {
        curr = tqueue->head;
        while (curr != NULL) {
            if (curr->tid == tid) {
                return curr;
            }
            curr = curr->next;
        }
    }
    return curr;
}

task_t* remove_task(task_queue_t* tqueue, int tid) {
    task_t *curr = NULL, *prev = NULL;
    if (tqueue != NULL) {
        curr = tqueue->head;
        while (curr != NULL) {
            if (curr->tid == tid) {
                if (curr == tqueue->head) {
                    tqueue->head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                
                if (curr == tqueue->tail) {
                    tqueue->tail = prev;
                }
                curr->next = NULL;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    return curr;
}

