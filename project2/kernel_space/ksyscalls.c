#include "kutils.h"
#include "kscheduler.h"
#include "kresource.h"
#include "kqueue.h"

#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/mutex.h>

extern struct mutex pqueues_lock;
extern struct mutex resources_lock;
extern struct mutex waiting_queue_lock;
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

    mutex_lock(&pqueues_lock);
    to_pqueues(task);
    mutex_unlock(&pqueues_lock);

    return 0;
}

SYSCALL_DEFINE2(add_resource, int, rid, int, quantity) {
    printk(KERN_INFO "Syscall Adding resource %d with quantity %d\n", rid, quantity);
    mutex_lock(&resources_lock);
    enqueue_resource(resources, create_resource(rid, quantity));
    mutex_unlock(&resources_lock);
    return 0;
}

SYSCALL_DEFINE0(schedule_tasks) {
    start_scheduler();
    return 0;
}

SYSCALL_DEFINE0(print_pq) {
    mutex_lock(&pqueues_lock);
    print_pqueues(pqueues);
    mutex_unlock(&pqueues_lock);
    return 0;
}

SYSCALL_DEFINE0(print_wq) {
    mutex_lock(&waiting_queue_lock);
    print_tqueue(waiting_queue);
    mutex_unlock(&waiting_queue_lock);
    return 0;
}

SYSCALL_DEFINE0(print_rq) {
    mutex_lock(&resources_lock);
    print_rqueue(resources);
    mutex_unlock(&resources_lock);
    return 0;
}
