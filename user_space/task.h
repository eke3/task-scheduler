#ifndef TASK_H
#define TASK_H

#include "resource.h"

typedef enum task_priority {
    HIGH = 2,
    MEDIUM = 1,
    LOW = 0
} task_priority_t;

typedef struct task {
    int tid;
    task_priority_t priority;
    int duration;
    resource_t* resources; // Array of resources.
    struct task* next;
} task_t;

#ifdef __cplusplus
extern "C" {
#endif

extern task_t* create_task(int tid, task_priority_t priority, int duration, resource_t* resources);

extern void increase_priority(task_t* task);


#ifdef __cplusplus
}
#endif

#endif