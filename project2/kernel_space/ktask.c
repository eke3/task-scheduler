#include "ktask.h"

#include <linux/slab.h>

task_t* create_task(int tid, task_priority_t priority, int duration, int* resources, size_t num_resources) {
    task_t* task;
    
    if (tid <=0 || duration <= 0 || num_resources < 0) {
        return NULL;
    }

    if ((task = (task_t*)kmalloc(sizeof(task_t), GFP_KERNEL)) == NULL) {
        return NULL;
    }

    if (resources == NULL) {
        task->resources = (int*)kmalloc(sizeof(int), GFP_KERNEL);
        task->num_resources = 0;
    } else {
        task->resources = resources;
        task->num_resources = num_resources;
    }

    task->tid = tid;
    task->priority = priority;
    task->duration = duration;
    // task->resources = resources;
    // task->num_resources = num_resources;
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