#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "utils.h"


#ifdef __cplusplus
extern "C" {
#endif


extern void execute_task(task_t* task);

extern void schedule_tasks(void);

extern void process_pqueue(task_queue_t* pqueue);

extern void process_waiting_queue(void);



#ifdef __cplusplus
}
#endif

#endif