#include "kenvironment.h"

#include "kutils.h"
#include <linux/mutex.h>
#include <linux/syscalls.h>

struct mutex pqueues_lock;
struct mutex resources_lock;
struct mutex waiting_queue_lock;
resource_queue_t* resources; // Queue for existing resources.
priority_queues_t* pqueues; // Priority queues (HIGH, MEDIUM, LOW).
task_queue_t* waiting_queue; // Queue for waiting tasks.

SYSCALL_DEFINE0(set_up_scheduler) {
    set_up();
    return 0;
}

SYSCALL_DEFINE0(tear_down_scheduler) {
    tear_down();
    return 0;
}

void set_up() {
    pqueues = create_priority_queues();
    waiting_queue = create_task_queue();
    resources = create_resource_queue();
    mutex_init(&pqueues_lock);
    mutex_init(&resources_lock);
    mutex_init(&waiting_queue_lock);

    if (pqueues == NULL || waiting_queue == NULL || resources == NULL) {
        printk(KERN_ERR "set_up() failed: Memory allocation error.\n");
    }
}

void tear_down() {
    free_priority_queues(pqueues);
    free_task_queue(waiting_queue);
    free_resource_queue(resources);
}