#ifndef KSCHEDULER_H
#define KSCHEDULER_H

#define TASK_OLD_AGE 100

#include "kutils.h"

// void execute_task(task_t* task)
// Description: Executes a task.
// Preconditions: A valid task pointer is passed as an argument.
// Postconditions: The task is executed.
// Returns: None.
extern void execute_task(task_t* task);

// void process_pqueue(task_queue_t* pqueue)
// Description: Processes a priority queue.
// Preconditions: A valid priority queue pointer is passed as an argument.
// Postconditions: Tasks in the queue are executed or moved to the waiting queue.
// Returns: None.
extern void process_pqueue(task_queue_t* pqueue);

// void process_waiting_queue()
// Description: Processes the waiting queue.
// Preconditions: The environment is set up.
// Postconditions: Waiting tasks are moved to the appropriate queue.
// Returns: None.
extern int process_waiting_queue(void);

// void schedule_tasks()
// Description: Runs the task scheduler.
// Preconditions: The environment is set up.
// Postconditions: Queued tasks are scheduled and run according to their priority.
// Returns: None.
extern void schedule_tasks(void);

#endif