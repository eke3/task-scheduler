#ifndef KQUEUE_H
#define KQUEUE_H

#include "ktask.h"

typedef struct resource_queue {
    resource_t* head;
    resource_t* tail;
} resource_queue_t;

typedef struct task_queue {
    task_t* head;
    task_t* tail;
} task_queue_t;

typedef struct priority_queues {
    task_queue_t* high;
    task_queue_t* medium;
    task_queue_t* low;
} priority_queues_t;

// priority_queues_t* create_priority_queues()
// Description: Creates a new priority queues.
// Preconditions: None.
// Postconditions: A new priority queues is created.
// Returns: A pointer to the new priority queues.
extern priority_queues_t* create_priority_queues(void);

// resource_queue_t* create_resource_queue()
// Description: Creates a new resource queue.
// Preconditions: None.
// Postconditions: A new resource queue is created.
// Returns: A pointer to the new resource queue.
extern resource_queue_t* create_resource_queue(void);

// task_queue_t* create_task_queue()
// Description: Creates a new task queue.
// Preconditions: None.
// Postconditions: A new task queue is created.
// Returns: A pointer to the new task queue.
extern task_queue_t* create_task_queue(void);

// task_t* dequeue_task(task_queue_t* tqueue)
// Description: Removes a task from a task queue.
// Preconditions: A valid task queue pointer is passed as an argument.
// Postconditions: The oldest task is removed from the task queue.
// Returns: A pointer to the removed task.
extern task_t* dequeue_task(task_queue_t* tqueue);

// void enqueue_resource(resource_queue_t* rqueue, resource_t* resource)
// Description: Adds a resource to a resource queue.
// Preconditions: A valid resource queue pointer and a valid resource pointer are passed as arguments.
// Postconditions: The resource is added to the resource queue.
// Returns: None.
extern void enqueue_resource(resource_queue_t* rqueue, resource_t* resource);

// void enqueue_task(task_queue_t* tqueue, task_t* task)
// Description: Adds a task to a task queue.
// Preconditions: A valid task queue pointer and a valid task pointer are passed as arguments.
// Postconditions: The task is added to the task queue.
// Returns: None.
extern void enqueue_task(task_queue_t* tqueue, task_t* task);

// void free_priority_queues(priority_queues_t* pqueues)
// Description: Frees a priority queues.
// Preconditions: A valid priority queues pointer is passed as an argument.
// Postconditions: The priority queues are freed.
// Returns: None.
extern void free_priority_queues(priority_queues_t* pqueues);

// void free_resource_queue(resource_queue_t* rqueue)
// Description: Frees a resource queue.
// Preconditions: A valid resource queue pointer is passed as an argument.
// Postconditions: The resource queue is freed.
// Returns: None.
extern void free_resource_queue(resource_queue_t* rqueue);

// void free_task_queue(task_queue_t* tqueue)
// Description: Frees a task queue.
// Preconditions: A valid task queue pointer is passed as an argument.
// Postconditions: The task queue is freed.
// Returns: None.
extern void free_task_queue(task_queue_t* tqueue);

// resource_t* find_resource_id(resource_queue_t* rqueue, int rid)
// Description: Finds a resource in a resource queue by its ID.
// Preconditions: A valid resource queue pointer and a resource ID are passed as arguments.
// Postconditions: None.
// Returns: A pointer to the found resource.
extern resource_t* find_resource_id(resource_queue_t* rqueue, int rid);

// task_t* find_task_id(task_queue_t* tqueue, int tid)
// Description: Finds a task in a task queue by its ID.
// Preconditions: A valid task queue pointer and a task ID are passed as arguments.
// Postconditions: None.
// Returns: A pointer to the found task.
extern task_t* find_task_id(task_queue_t* tqueue, int tid);

// task_t* remove_task(task_queue_t* tqueue, int tid)
// Description: Removes a task from a task queue.
// Preconditions: A valid task queue pointer and a task ID are passed as arguments.
// Postconditions: The task with the specified ID is removed from the task queue.
// Returns: A pointer to the removed task.
extern task_t* remove_task(task_queue_t* tqueue, int tid);

#endif