#ifndef QUEUE_H
#define QUEUE_H


// structs, constants, #defines, globals, #includes


#include <semaphore.h>

#define FAILURE -1
#define HIGH_PRIORITY 2
#define MEDIUM_PRIORITY 1
#define LOW_PRIORITY 0


typedef struct task {
    int tid;
    int priority;
    int duration;
    resource_t* resources;
    task_t* next;
} task_t;

typedef struct resource {
    int rid;
    sem_t* semaphore;
    resource_t* next;
} resource_t;

typedef struct task_queue {
    task_t* tasks;
} task_queue_t;

typedef struct resource_queue {
    resource_t* resources;
} resource_queue_t;



#ifdef __cplusplus
extern "C" {
#endif


// function prototypes
extern int enqueue_task(task_queue_t* queue, task_t* task);
extern int enqueue_resource(resource_queue_t* queue, resource_t* resource);

extern task_t* dequeue_task(task_queue_t* queue);
// extern resource_t* dequeue_resource(resource_queue_t* queue);

extern task_t* remove_task(task_queue_t* queue, int tid);
extern resource_t* remove_resource(resource_queue_t* queue, int rid);

extern int acquire_resources(task_t* task, resource_queue_t* queue);
extern int release_resources(task_t* task, resource_queue_t* queue);
extern int lock_resource(resource_t* resource, resource_queue_t* queue);
extern int unlock_resource(resource_t* resource, resource_queue_t* queue);

extern int increase_priority(task_t* task);

// extern task_t* peek_task(task_queue_t* queue);
// extern resource_t* peek_resource(resource_queue_t* queue);

extern int is_empty_task(task_queue_t* queue);
// extern int is_empty_resource(resource_queue_t* queue);

// FOR DEBUGGING
extern void print_task_queue(task_queue_t* queue);
extern void print_resource_queue(resource_queue_t* queue);

extern task_t* find_task(task_queue_t* queue, int tid);
extern resource_t* find_resource(resource_queue_t* queue, int rid);


// for queues
extern task_queue_t* create_task_queue();
extern resource_queue_t* create_resource_queue();
extern void free_task_queue(task_queue_t* queue);
extern void free_resource_queue(resource_queue_t* queue);





#ifdef __cplusplus
}
#endif





#endif // QUEUE_H