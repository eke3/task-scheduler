// File:    task.c
// Author:  Eric Ekey
// Date:    03/19/2025
// Desc:    This file contains functions for creating and modifying tasks.

#include "task.h"

#include <stdlib.h>

task_t* create_task(int tid, task_priority_t priority, int duration, int* resources, size_t num_resources) {
    if (tid <=0 || duration <= 0 || num_resources < 0) {
        return NULL;
    }

    task_t* task;
    if ((task = malloc(sizeof(task_t))) == NULL) {
        return NULL;
    }

    task->tid = tid;
    task->priority = priority;
    task->duration = duration;
    task->resources = resources;
    task->num_resources = num_resources;
    task->age = 0;
    task->next = NULL;
    return task;
}

void increase_priority(task_t* task) {
    if (task) {
        // Increase task priority by one step, unless it is already the highest priority.
        switch (task->priority) {
            case HIGH:
                break;
            case MEDIUM:
                task->priority = HIGH;
                break;
            case LOW:
                task->priority = MEDIUM;
                break;
        }
    }
}