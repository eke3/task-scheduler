#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

// void acquire_resources(task_t* task)
// Description: Acquires the resources needed by a task.
// Preconditions: A valid task pointer is passed as an argument.
// Postconditions: The task's required resources are acquired if available.
// Returns: None.
extern void acquire_resources(task_t* task);

// bool are_there_any_uncompleted_tasks_left()
// Description: Checks if there are any uncompleted tasks left in the scheduler.
// Preconditions: None.
// Postconditions: None.
// Returns: True if there are any uncompleted tasks left, false otherwise.
extern bool are_there_any_uncompleted_tasks_left(void);

// bool are_there_any_waiting_tasks_left()
// Description: Checks if there are any waiting tasks left in the scheduler.
// Preconditions: None.
// Postconditions: None.
// Returns: True if there are any tasks in the waiting queue, false otherwise.
extern bool are_there_any_waiting_tasks_left(void);

// bool can_acquire_resources(task_t* task)
// Description: Checks if a task can acquire the resources it needs.
// Preconditions: A valid task pointer is passed as an argument.
// Postconditions: None.
// Returns: True if the task's required resources are available, false otherwise.
extern bool can_acquire_resources(task_t* task);

// void print_pqueues(priority_queues_t* pqueues)
// Description: Prints the priority queues.
// Preconditions: A valid priority queues pointer is passed as an argument.
// Postconditions: The priority queues are printed to the console.
// Returns: None.
extern void print_pqueues(priority_queues_t* pqueues);

// void print_rqueue(resource_queue_t* rqueue)
// Description: Prints a resource queue.
// Preconditions: A valid resource queue pointer is passed as an argument.
// Postconditions: The resource queue is printed to the console.
// Returns: None.
extern void print_rqueue(resource_queue_t* rqueue);

// void print_tqueue(task_queue_t* tqueue)
// Description: Prints a task queue.
// Preconditions: A valid task queue pointer is passed as an argument.
// Postconditions: The task queue is printed to the console.
// Returns: None.
extern void print_tqueue(task_queue_t* tqueue);

// void release_resources(task_t* task)
// Description: Releases the resources acquired by a task.
// Preconditions: A valid task pointer is passed as an argument.
// Postconditions: The task's acquired resources are released.
// Returns: None.
extern void release_resources(task_t* task);

// void to_pqueues(task_t* task)
// Description: Adds a task to a priority queue corresponding to its priority.
// Preconditions: A valid task pointer is passed as an argument.
// Postconditions: The task is added to a HIGH/MEDIUM/LOW priority queue.
// Returns: None.
extern void to_pqueues(task_t* task);

#ifdef __cplusplus
}
#endif

#endif
