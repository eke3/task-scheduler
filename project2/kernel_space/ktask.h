#ifndef KTASK_H
#define KTASK_H

#include "kresource.h"

typedef enum task_priority {
    HIGH = 2,
    MEDIUM = 1,
    LOW = 0
} task_priority_t;

typedef struct task {
    int tid;
    task_priority_t priority;
    int duration;
    int* resources;
    size_t num_resources;
    int age;
    struct task* next;
} task_t;

// task_t* create_task(int tid, task_priority_t priority, int duration, resource_t* resources)
// Description: Creates a new task.
// Preconditions: Valid task parameters are passed as arguments.
// Postconditions: A new task is created.
// Returns: A pointer to the created task.
extern task_t* create_task(int tid, task_priority_t priority, int duration, int* resources, size_t num_resources);

// void increase_priority(task_t* task)
// Description: Increases the priority of a task.
// Preconditions: A valid task pointer is passed as an argument.
// Postconditions: The task's priority is increased by one step.
// Returns: None.
extern void increase_priority(task_t* task);

#endif