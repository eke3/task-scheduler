#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "queue.h"

priority_queues_t* task_queues;

task_queue_t* waiting_tasks;
task_queue_t* completed_tasks;

resource_queue_t* resources;



#ifdef __cplusplus
extern "C" {
#endif

extern int set_up(void);

extern int tear_down(void);

extern int has_pending_tasks(void);


#ifdef __cplusplus
}
#endif



#endif // ENVIRONMENT_H