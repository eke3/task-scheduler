#ifndef QUEUE_H
#define QUEUE_H

#include "task.h"

// Linked list queue for holding tasks.
typedef struct task_queue {
    task_t* head;
    task_t* tail;
} task_queue_t;

// Linked list queue for holding resources.
typedef struct resource_queue {
    resource_t* head;
    resource_t* tail;
} resource_queue_t;

// Struct for holding three task queues of cascading priority.
typedef struct priority_queues {
    task_queue_t* high;
    task_queue_t* medium;
    task_queue_t* low;
} priority_queues_t;

#ifdef __cplusplus
extern "C" {
#endif

// Queue creation functions.
extern task_queue_t* create_task_queue(void);
extern resource_queue_t* create_resource_queue(void);
extern priority_queues_t* create_priority_queues(void);


// Queue deletion functions.
extern void free_task_queue(task_queue_t* tqueue);
extern void free_resource_queue(resource_queue_t* rqueue);
extern void free_priority_queues(priority_queues_t* pqueues);


// Queue insertion functions.
extern void enqueue_task(task_queue_t* tqueue, task_t* task);
extern void enqueue_resource(resource_queue_t* rqueue, resource_t* resource);

// Queue removal functions.
extern task_t* remove_task(task_queue_t* tqueue, int tid);

// Queue searching functions.
// extern task_t* find_task(task_queue_t* tqueue, int tid);
extern resource_t* find_resource_id(resource_queue_t* rqueue, int rid);


#ifdef __cplusplus
}
#endif

#endif
