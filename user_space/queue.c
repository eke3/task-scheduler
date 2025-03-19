#include "queue.h"

#include <stdlib.h>

// Queue creation functions.
task_queue_t* create_task_queue() {
    task_queue_t* tqueue;
    if ((tqueue = malloc(sizeof(task_queue_t))) == NULL) {
        return NULL;
    }

    tqueue->head = NULL;
    tqueue->tail = NULL;
    return tqueue;
}

resource_queue_t* create_resource_queue() {
    resource_queue_t* rqueue;
    if ((rqueue = malloc(sizeof(resource_queue_t))) == NULL) {
        return NULL;
    }

    rqueue->head = NULL;
    rqueue->tail = NULL;
    return rqueue;
}

priority_queues_t* create_priority_queues() {
    priority_queues_t* pqueues;
    if ((pqueues = malloc(sizeof(priority_queues_t))) == NULL) {
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


// Queue deletion functions.
void free_task_queue(task_queue_t* tqueue) {
    if (tqueue) {
        task_t* curr = tqueue->head;
        task_t* prev = NULL;
        while (curr != NULL) {
            prev = curr;
            curr = curr->next;
            free(prev);
        }
        free(tqueue);
    }
}

void free_resource_queue(resource_queue_t* rqueue) {
    if (rqueue) {
        resource_t* curr = rqueue->head;
        resource_t* prev = NULL;
        while (curr != NULL) {
            prev = curr;
            curr = curr->next;
            free(prev);
        }
        free(rqueue);
    }
}

void free_priority_queues(priority_queues_t* pqueues) {
    if (pqueues) {
        free_task_queue(pqueues->high);
        free_task_queue(pqueues->medium);
        free_task_queue(pqueues->low);
        free(pqueues);
    }
}


 // Queue insertion functions.
void enqueue_task(task_queue_t* tqueue, task_t* task) {
    if (tqueue && task) {
        if (tqueue->head == NULL) {
            tqueue->head = task;
            tqueue->tail = task;
        } else {
            tqueue->tail->next = task;
            tqueue->tail = task;
        }
    }
}

void enqueue_resource(resource_queue_t* rqueue, resource_t* resource) {
    if (rqueue && resource) {
        if (rqueue->head == NULL) {
            rqueue->head = resource;
            rqueue->tail = resource;
        } else {
            rqueue->tail->next = resource;
            rqueue->tail = resource;
        }
    }
}


// Queue removal functions.
task_t* remove_task(task_queue_t* tqueue, int tid) {
    if (tqueue) {
        task_t* curr = tqueue->head;
        task_t* prev = NULL;
        
        while (curr != NULL) {
            if (curr->tid == tid) {
                if (prev == NULL) {
                    tqueue->head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                if (tqueue->tail == curr) {
                    tqueue->tail = prev;
                }
                return curr;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    return NULL;
}


// Queue searching functions.
resource_t* find_resource_id(resource_queue_t* rqueue, int rid) {
    if (rqueue) {
        resource_t* curr = rqueue->head;
        while (curr != NULL) {
            if (curr->rid == rid) {
                return curr;
            }
            curr = curr->next;
        }
    }
    return NULL;
}