#include <kenvironment.h>

#include <linux/mutex.h>
#include <stdlib.h>

struct mutex pqueues_lock;
struct mutex resources_lock;
struct mutex waiting_queue_lock;
resource_queue_t* resources; // Queue for existing resources.
priority_queues_t* pqueues; // Priority queues (HIGH, MEDIUM, LOW).
task_queue_t* waiting_queue; // Queue for waiting tasks.

void set_up() {
    pqueues = create_priority_queues();
    waiting_queue = create_task_queue();
    resources = create_resource_queue();
    mutex_init(&pqueues_lock);
    mutex_init(&resources_lock);
    mutex_init(&waiting_queue_lock);

    if (pqueues == NULL || waiting_queue == NULL || resources == NULL) {
        tear_down();
        exit(EXIT_FAILURE);
    }
}

void tear_down() {
    free_priority_queues(pqueues);
    free_task_queue(waiting_queue);
    free_resource_queue(resources);
}