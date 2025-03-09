#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"

extern priority_queues_t* task_queues;

extern task_queue_t* waiting_tasks;
extern task_queue_t* completed_tasks;

extern resource_queue_t* resources;

#ifdef __cplusplus
extern "C" {
#endif


extern int run_scheduler(void);

extern void* thread_execute_task(void*);

extern int process_queue(task_queue_t*);

extern int check_waiting(void);

extern void append_thread(pthread_t);

extern void cleanup_thread_table(void);

#ifdef __cplusplus
}
#endif

#endif // SCHEDULER_H