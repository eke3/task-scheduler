#ifndef UTILS_H
#define UTILS_H

#include "queue.h"

#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

extern bool are_there_any_uncompleted_tasks_left(void);

extern bool can_acquire_resources(task_t* task);

extern void acquire_resources(task_t* task);

extern void release_resources(task_t* task);

extern void to_pqueues(task_t* task);

extern void print_tqueue(task_queue_t* tqueue);

extern void print_rqueue(resource_queue_t* rqueue);

extern void print_pqueues(priority_queues_t* pqueues);

#ifdef __cplusplus
}
#endif

#endif