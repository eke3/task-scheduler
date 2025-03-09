#include "scheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "environment.h"

int run_scheduler(void) {

    printf("running scheduler\n");
    printf("there are %s pending tasks\n", (has_pending_tasks()) == 1 ? "" : "no");

    while (has_pending_tasks() == 0) {
        printf("enter loop\n");
        // go through high priority queue.
        if (process_queue(task_queues->high_priority_tasks) == FAILURE) {
            return FAILURE;
        }


        // repeat for mid pririty queue
        if (process_queue(task_queues->medium_priority_tasks) == FAILURE) {
            return FAILURE;
        }


        // repeat for low priority queue
        if (process_queue(task_queues->low_priority_tasks) == FAILURE) {
            return FAILURE;
        }




        // go through waiting queue.
        // make list of tid's that successfully acquire resources
        // move them back to the priority queues
        if (check_waiting() == FAILURE) {
            return FAILURE;
        }
        
        break;

    }

    if (has_pending_tasks() == FAILURE) {
        return FAILURE;
    }
    return 0;
}

int execute_task(task_t* task) {
    if (task == NULL) {
        return FAILURE;
    }

    printf("executing task %d\n", task->tid);
    return sleep(task->duration); // returns seconds remaining if interrupted, or 0 if completed successfully
    // return 0;
}

int process_queue(task_queue_t* queue) {
    if (queue == NULL) {
        return FAILURE;
    }
    // make list of tid's that successfully acquire resources and tid's that fail
    // need to dynamically expand list
    // execute tasks that got resources
    // move successfully executed tasks to completed queue
    // move failed tasks to waiting queue


    // go back and check these for calloc failures
    size_t capacity_successful = 10, capacity_failed = 10;
    int i = 0, j = 0;
    int* successful_tids = calloc(capacity_successful, sizeof(int));
    int* failed_tids = calloc(capacity_failed, sizeof(int));


    task_t* curr = queue->tasks;
    while (curr != NULL) {
        // check if we need to expand arrays
        if (i >= capacity_successful) {
            size_t new_capacity = capacity_successful * 2;
            int* new_successful = calloc(new_capacity, sizeof(int));  // Zero initialized
            if (new_successful) {
                memcpy(new_successful, successful_tids, capacity_successful * sizeof(int));  // Copy old data
                free(successful_tids);
                successful_tids = new_successful;
                capacity_successful = new_capacity;
            }
        }

        if (j >= capacity_failed) {
            size_t new_capacity = capacity_failed * 2;
            int* new_failed = calloc(new_capacity, sizeof(int));  // Zero initialized
            if (new_failed) {
                memcpy(new_failed, failed_tids, capacity_failed * sizeof(int));  // Copy old data
                free(failed_tids);
                failed_tids = new_failed;
                capacity_failed = new_capacity;
            }
        }

        if (acquire_resources(curr, resources) != FAILURE) {
            // successfuly acquired resources
            successful_tids[i] = curr->tid;
            i++;
        } else {
            // failed to acquire resources
            failed_tids[j] = curr->tid;
            j++;
        }
        curr = curr->next;
    }

    // now theres 2 populated lists of tids

    // execute tasks that got resources
    // move successfully executed tasks to completed queue
    // move failed tasks to waiting queue

    int k = 0;
    while (k < capacity_successful) {
        if (successful_tids[k] > 0) {
            // get task
            task_t* task_to_exec = find_task(queue, successful_tids[k]);
            // execute task
            if (execute_task(task_to_exec) == FAILURE) {
                return FAILURE;
            }
            // release resources
            release_resources(task_to_exec, resources);
            // move to completed queue
            remove_task(queue, task_to_exec->tid);
            enqueue_task(completed_tasks, task_to_exec);
        }
        k++;
    }

    int m = 0;
    while (m < capacity_failed) {
        if (failed_tids[m] > 0) {
            // get task
            task_t* task_to_wait = find_task(queue, failed_tids[m]);
            // move to waiting queue
            remove_task(queue, task_to_wait->tid);
            enqueue_task(waiting_tasks, task_to_wait);
        }
        m++;
    }

    free(successful_tids);
    free(failed_tids);    

    return 0;
}

int check_waiting(void) {
    task_t* curr = waiting_tasks->tasks;
    while (curr != NULL) {
        if (acquire_resources(curr, resources) != FAILURE) {
            // successfully acquired resources
            // move back to priority queues
            // release resources
            release_resources(curr, resources);
            remove_task(waiting_tasks, curr->tid);
            to_pqueue(curr);
        }
        curr = curr->next;
    }
    return 0;
}