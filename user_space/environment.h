#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "queue.h"

extern priority_queues_t* task_queues;

extern task_queue_t* waiting_tasks;
extern task_queue_t* completed_tasks;

extern resource_queue_t* resources;



#ifdef __cplusplus
extern "C" {
#endif

extern int set_up(void);

extern int tear_down(void);

extern int has_pending_tasks(void);

extern int to_pqueue(task_t*);


#ifdef __cplusplus
}
#endif



#endif // ENVIRONMENT_H