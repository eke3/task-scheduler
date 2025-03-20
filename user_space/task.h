#ifndef TASK_H
#define TASK_H

#include "resource.h"

// Enum for task priorities.
typedef enum task_priority {
    HIGH = 2,
    MEDIUM = 1,
    LOW = 0
} task_priority_t;

// Struct for holding a task's information.
typedef struct task {
    int tid;
    task_priority_t priority;
    int duration;
    // Array of task's required resources.
    resource_t* resources;
    struct task* next;
} task_t;

#ifdef __cplusplus
extern "C" {
#endif

// task_t* create_task(int tid, task_priority_t priority, int duration, resource_t* resources)
// Description: Creates a new task.
// Preconditions: Valid task parameters are passed as arguments.
// Postconditions: A new task is created.
// Returns: A pointer to the created task.
extern task_t* create_task(int tid, task_priority_t priority, int duration, resource_t* resources);

// void increase_priority(task_t* task)
// Description: Increases the priority of a task.
// Preconditions: A valid task pointer is passed as an argument.
// Postconditions: The task's priority is increased by one step.
// Returns: None.
extern void increase_priority(task_t* task);

#ifdef __cplusplus
}
#endif

#endif