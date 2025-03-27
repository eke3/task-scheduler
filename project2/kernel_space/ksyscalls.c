#include "kutils.h"

#include <linux/uaccess.h>
#include <linux/syscalls.h>

extern resource_queue_t* resources;
extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;

SYSCALL_DEFINE5(add_task, int, tid, task_priority_t, priority, int, duration, int __user *, resources, size_t, num_resources) {
    int* resources_array = NULL;
    task_t* task = NULL;

    resources_array = (int*)kmalloc(num_resources*2*sizeof(int), GFP_KERNEL);

    if (copy_from_user(resources_array, resources, num_resources*2*sizeof(int))) {
        return -EINVAL;
    }

    task = create_task(tid, priority, duration, resources_array, num_resources);

    to_pqueues(task);

    return 0;
}

SYSCALL_DEFINE0(print_priority_queues) {
    print_pqueues(pqueues);
    return 0;
}

SYSCALL_DEFINE0(print_waiting_queue) {
    print_tqueue(waiting_queue);
    return 0;
}

SYSCALL_DEFINE0(print_resource_queue) {
    print_rqueue(resources);
    return 0;
}