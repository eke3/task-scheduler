#include "environment.h"

priority_queues_t* task_queues;

task_queue_t* waiting_tasks;
task_queue_t* completed_tasks;

resource_queue_t* resources;



void scene_1(void);
void scene_2(void);
void scene_3(void);

int main() {
    // set up
    if (set_up() == FAILURE) {
        return FAILURE;
    }

    scene_1();
    
}