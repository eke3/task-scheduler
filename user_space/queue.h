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

typedef struct priority_queues {
    task_queue_t* high_priority_tasks;
    task_queue_t* medium_priority_tasks;
    task_queue_t* low_priority_tasks;
} priority_queues_t;



#ifdef __cplusplus
extern "C" {
#endif


// function prototypes
extern int enqueue_task(task_queue_t*, task_t*);
extern int enqueue_resource(resource_queue_t*, resource_t*);

extern task_t* dequeue_task(task_queue_t*);
// extern resource_t* dequeue_resource(resource_queue_t* queue);

extern task_t* remove_task(task_queue_t*, int);
extern resource_t* remove_resource(resource_queue_t*, int);

extern int acquire_resources(task_t*, resource_queue_t*);
extern int release_resources(task_t*, resource_queue_t*);
extern int lock_resource(resource_t*, resource_queue_t*);
extern int unlock_resource(resource_t*, resource_queue_t*);

extern int increase_priority(task_t*);

// extern task_t* peek_task(task_queue_t* queue);
// extern resource_t* peek_resource(resource_queue_t* queue);

extern int has_task(task_queue_t*);
// extern int is_empty_resource(resource_queue_t* queue);

// FOR DEBUGGING
extern void print_task_queue(task_queue_t*);
extern void print_resource_queue(resource_queue_t*);

extern task_t* find_task(task_queue_t*, int);
extern resource_t* find_resource(resource_queue_t*, int);


// for queues
extern priority_queues_t* create_priority_queues(void);
extern task_queue_t* create_task_queue(void);
extern resource_queue_t* create_resource_queue(void);
extern void free_task_queue(task_queue_t*);
extern void free_resource_queue(resource_queue_t*);
extern int free_priority_queues(priority_queues_t*);

int to_pqueue(priority_queues_t*, task_t*);



#ifdef __cplusplus
}
#endif





#endif // QUEUE_H