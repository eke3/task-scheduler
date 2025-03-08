#include "queue.h"

#include <stdlib.h>

task_queue_t* create_task_queue() {
    task_queue_t* queue;
    if ((queue = malloc(sizeof(task_queue_t))) == NULL) {
        return NULL;
    }
    queue->tasks = NULL;
    return queue;
}

resource_queue_t* create_resource_queue() {
    resource_queue_t* queue;
    if ((queue = malloc(sizeof(resource_queue_t))) == NULL) {
        return NULL;
    }
    queue->resources = NULL;
    return queue;
}

int free_task_queue(task_queue_t* queue) {
    if (queue == NULL) {
        return FAILURE;
    }

    task_t* task = queue->tasks;

    while (task != NULL) {
        free(task);
        task = task->next;
    }

    return 0;
}


int free_resource_queue(resource_queue_t* queue) {
    if (queue == NULL) {
        return FAILURE;
    }

    resource_t* resource = queue->resources;

    while (resource != NULL) {
        free(resource);
        resource = resource->next;
    }

    return 0;
}

int is_empty_task(task_queue_t* queue) {
    if (queue == NULL) {
        return FAILURE;
    }

    return queue->tasks == NULL;
}

void print_task_queue(task_queue_t* queue) {
    task_t* task = queue->tasks;

    while (task != NULL) {
        printf("{\ntid: %d\npriority: %d\nduration: %d\n}\n\n", task->tid, task->priority, task->duration);
    }
}

void print_resource_queue(resource_queue_t* queue) {
    resource_t* resource = queue->resources;

    while (resource != NULL) {
        printf("{\nrid: %d\n}\n\n", resource->rid);
    }
}

int enqueue_task(task_queue_t* queue, task_t* task) {
    if (queue == NULL || task == NULL) {
        return FAILURE;
    }

    if (!is_empty_task(queue)) {
        task_t* current = queue->tasks;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = task;
    } else {
        queue->tasks = task;
    }
    return 0;
}

int enqueue_resource(resource_queue_t* queue, resource_t* resource) {
    if (queue == NULL || resource == NULL) {
        return FAILURE;
    }

    if (!is_empty_resource(queue)) {
        resource_t* current = queue->resources;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = resource;
    } else {
        if (find_resource(queue, resource->rid) != NULL) {
            // resource in queue, increment semaphore


        } else {
            queue->resources = resource;
        }
    }
    return 0;
}

task_t* dequeue_task(task_queue_t* queue) {
    if (queue == NULL) {
        return NULL;
    }

    task_t* task = queue->tasks;
    queue->tasks = task->next;
    return task;
}

task_t* remove_task(task_queue_t* queue, int tid) {
    if (queue == NULL) {
        return NULL;
    }

    task_t* task = queue->tasks;
    task_t* prev = NULL;

    while (task != NULL) {
        if (task->tid == tid) {
            if (prev == NULL) {
                queue->tasks = task->next;
            } else {
                prev->next = task->next;
            }
            return task;
        }
        prev = task;
        task = task->next;
    }

    return NULL;
}

resource_t* remove_resource(resource_queue_t* queue, int rid) {
    if (queue == NULL) {
        return NULL;
    }

    resource_t* resource = queue->resources;
    resource_t* prev = NULL;

    while (resource != NULL) {
        if (resource->rid == rid) {
            if (prev == NULL) {
                queue->resources = resource->next;
            } else {
                prev->next = resource->next;
            }
            return resource;
        }
        prev = resource;
        resource = resource->next;
    }

    return NULL;
}

int increase_priority(task_t* task) {
    if (task == NULL) {
        return FAILURE;
    }
    
    if (task->priority == LOW_PRIORITY || task->priority == MEDIUM_PRIORITY) {
        task->priority++;
    }

    return 0;
}

// resource_t* dequeue_resource(resource_queue_t* queue) {
//     if (queue == NULL) {
//         return NULL;
//     }

//     resource_t* resource = queue->resources;
//     queue->resources = resource->next;
//     return resource;
// }

// task_t* peek_task(task_queue_t* queue) {
//     if (queue == NULL) {
//         return NULL;
//     }

//     return queue->tasks;
// }

// resource_t* peek_resource(resource_queue_t* queue) {
//     if (queue == NULL) {
//         return NULL;
//     }

//     return queue->resources;
// }

task_t* find_task(task_queue_t* queue, int tid) {
    if (queue == NULL) {
        return NULL;
    }

    task_t* task = queue->tasks;

    while (task != NULL) {
        if (task->tid == tid) {
            return task;
        }
        task = task->next;
    }

    return NULL;
}

resource_t* find_resource(resource_queue_t* queue, int rid) {
    if (queue == NULL) {
        return NULL;
    }

    resource_t* resource = queue->resources;

    while (resource != NULL) {
        if (resource->rid == rid) {
            return resource;
        }
        resource = resource->next;
    }

    return NULL;
}

int acquire_resources(task_t* task, resource_queue_t* queue) {
    if (task == NULL || queue == NULL) {
        return FAILURE;
    }

    resource_t* resource = task->resources;

    while (resource != NULL) {
        int sem_val_1, sem_val_2;
        if (sem_getvalue(resource->semaphore, &sem_val_1) != 0) {
            perror("sem_getvalue error in acquire_resources()");
            return FAILURE;
        }
        resource_t* search_resource;
        if ((search_resource = find_resource(queue, resource->rid)) == NULL) {
            return FAILURE;
        } else {
            if (sem_getvalue(search_resource->semaphore, &sem_val_2) != 0) {
                perror("sem_getvalue error in acquire_resources()");
                return FAILURE;
            }
            if (sem_val_1 > sem_val_2) {
                return FAILURE;
            }
        }
        resource = resource->next;
    }

    resource = task->resources;

    while (resource != NULL) {
        if (lock_resource(resource, queue) == FAILURE) {
            return FAILURE;
        }
        resource = resource->next;
    }

    return 0;
}

// make sure the resource passed here is from the task and NOT the queue itself
int lock_resource(resource_t* resource, resource_queue_t* queue) {
    if (resource == NULL || queue == NULL) {
        return FAILURE;
    }

    int sem_val;
    if (sem_getvalue(resource->semaphore, &sem_val) != 0) {
        perror("sem_getvalue error in lock_resource()");
        return FAILURE;
    }
    
    resource_t* target_resource;

    if ((target_resource = find_resource(queue, resource->rid)) == NULL) {
        return FAILURE;
    }

    for (int i = 0; i < sem_val; i++) {
        if (sem_wait(target_resource->semaphore) != 0) {
            perror("sem_wait error in lock_resource()");
            return FAILURE;
        }
    }

    return 0;

}

int release_resources(task_t* task, resource_queue_t* queue) {
    if (task == NULL || queue == NULL) {
        return FAILURE;
    }

    resource_t* resource = task->resources;

    while (resource != NULL) {
        if (unlock_resource(resource, queue) == FAILURE) {
            return FAILURE;
        }
        resource = resource->next;
    }

    return 0;
}

int unlock_resource(resource_t* resource, resource_queue_t* queue) {
    if (resource == NULL || queue == NULL) {
        return FAILURE;
    }

    int sem_val;
    if (sem_getvalue(resource->semaphore, &sem_val) != 0) {
        perror("sem_getvalue error in unlock_resource()");
        return FAILURE;
    }
    
    resource_t* target_resource;

    if ((target_resource = find_resource(queue, resource->rid)) == NULL) {
        return FAILURE;
    }

    for (int i = 0; i < sem_val; i++) {
        if (sem_post(target_resource->semaphore) != 0) {
            perror("sem_post error in unlock_resource()");
            return FAILURE;
        }
    }
}