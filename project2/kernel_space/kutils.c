#include "kutils.h"

#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

extern resource_queue_t* resources;
extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;

SYSCALL_DEFINE1(sys_add_task, task_t __user *, task) {
    task_t* kernel_task;
    if ((kernel_task = (task_t*)kmalloc(sizeof(task_t), GFP_KERNEL)) != NULL) {
        if (copy_from_user(kernel_task, task, sizeof(task_t)) != 0) {
            kfree(kernel_task);
            return -EFAULT;
        }
        to_pqueues(kernel_task);
    } else {
        return -ENOMEM;
    }
    return 0;
}

SYSCALL_DEFINE1(sys_acquire_resources, task_t __user *, task) {
    task_t* kernel_task;
    if ((kernel_task = (task_t*)kmalloc(sizeof(task_t), GFP_KERNEL)) != NULL) {
        if (copy_from_user(kernel_task, task, sizeof(task_t)) != 0) {
            kfree(kernel_task);
            return -EFAULT;
        }
        acquire_resources(kernel_task);
        kfree(kernel_task);
    } else {
        return -ENOMEM;
    }
    return 0;
}

SYSCALL_DEFINE1(sys_release_resources, task_t __user *, task) {
    task_t* kernel_task;
    if ((kernel_task = (task_t*)kmalloc(sizeof(task_t), GFP_KERNEL)) != NULL) {
        if (copy_from_user(kernel_task, task, sizeof(task_t)) != 0) {
            kfree(kernel_task);
            return -EFAULT;
        }
        release_resources(kernel_task);
        kfree(kernel_task);
    } else {
        return -ENOMEM;
    }
    return 0;
}

void acquire_resources(task_t* task) {
    int i, j;
    int rid = 0;
    int num_needed = 0;
    resource_t* resource = NULL;

    if (task != NULL) {
        for (i = 0; i < (task->num_resources*2); i+=2) {
            rid = task->resources[i];
            num_needed = task->resources[i+1];
            resource = find_resource_id(resources, rid);
            for (j; j < num_needed; j++) {
                down(&resource->sem);
            }
        }
    }
}

int are_there_any_uncompleted_tasks_left() {
    // Check if there are any tasks in the priority queues.
    if (pqueues != NULL) {
        if (pqueues->high != NULL) {
            if (pqueues->high->head != NULL) {
                return 1;
            }
        }
        if (pqueues->medium != NULL) {
            if (pqueues->medium->head != NULL) {
                return 1;
            }
        }
        if (pqueues->low != NULL) {
            if (pqueues->low->head != NULL) {
                return 1;
            }
        }
    }
    return 0;
}

int are_there_any_waiting_tasks_left() {
    // Check if there are any tasks in the waiting queue.
    if (waiting_queue != NULL) {
        if (waiting_queue->head != NULL) {
            return 1;
        }
    }
    return 0;
}

int can_acquire_resources(task_t* task) {
    int can_acquire_success = -1;
    int i;
    int rid = 0;
    int num_needed = 0;
    int num_available = 0;
    resource_t* resource = NULL;

    if (task != NULL) {
        can_acquire_success = 0;
        if (task->resources != NULL) {
            for (i = 0; i < (task->num_resources*2); i+=2) {
                rid = task->resources[i];
                num_needed = task->resources[i+1];
                if ((resource = find_resource_id(resources, rid)) != NULL) {
                    num_available = resource->quantity;
                    if (num_available > num_needed) {
                        // Not enough of some resource
                        can_acquire_success = -1;
                        break;
                    }
                } else {
                    // Didn't find a required resource, task won't be able to run.
                    can_acquire_success = -1;
                    break;
                }
            }
        }
    }
    return can_acquire_success;
}

void print_pqueues(priority_queues_t* pqueues) {
    if (pqueues != NULL) {
        printf("Printing priority queues...\n");
        printf("High:\n");
        print_tqueue(pqueues->high);
        printf("Medium:\n");
        print_tqueue(pqueues->medium);
        printf("Low:\n");
        print_tqueue(pqueues->low);
    }
}

void print_rqueue(resource_queue_t* rqueue) {
    if (rqueue != NULL) {
        resource_t* curr = rqueue->head;
        printf("Printing resource queue...\n{\n");
        while (curr != NULL) {
            int quantity;
            sem_getvalue(curr->sem, &quantity);
            printf("\tResource ID: %d\tQuantity: %d\n", curr->rid, quantity);
            curr = curr->next;
        }
        printf("}\n");
    }
}

void print_tqueue(task_queue_t* tqueue) {
    if (tqueue != NULL) {
        task_t* curr = tqueue->head;
        printf("{\n");
        while (curr != NULL) {
            printf("\tTask ID: %d\n", curr->tid);
            curr = curr->next;
        }
        printf("}\n");
    }
}

void release_resources(task_t* task) {
    int i, j;
    int rid = 0;
    int num_to_release = 0;
    resource_t* resource = NULL;

    if (task != NULL) {
        for (i = 0; i < (task->num_resources*2); i+=2) {
            rid = task->resources[i];
            num_to_release = task->resources[i+1];
            resource = find_resource_id(resources, rid);
            for (j = 0; j < num_to_release; j++) {
                up(&resource->sem);
            }
        }
    }
}

void to_pqueues(task_t* task) {
    if (task != NULL) {
        switch (task->priority) {
            case HIGH:
                enqueue_task(pqueues->high, task);
                break;
            case MEDIUM:
                enqueue_task(pqueues->medium, task);
                break;
            case LOW:
                enqueue_task(pqueues->low, task);
                break;
        }
    }
}