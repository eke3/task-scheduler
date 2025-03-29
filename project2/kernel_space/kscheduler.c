#include "kscheduler.h"

#include "kenvironment.h"

#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include "kqueue.h"
#include "kutils.h"

#define NUM_CONSUMERS 5

extern struct mutex pqueues_lock;
extern struct mutex resources_lock;
extern struct mutex waiting_queue_lock;
extern resource_queue_t* resources;
extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;

struct task_struct* thread_pool[NUM_CONSUMERS];

void execute_task(task_t* task) {
    // Mimic task execution by sleeping for its duration.
    printk(KERN_INFO "Executing task %d...\n", task->tid);
    if (task) {
    msleep(task->duration*1000);
    }
    printk(KERN_INFO "Task %d completed\n", task->tid);
}

void process_pqueue(task_queue_t* pqueue) {
    task_t* task = NULL;
    while ((task = dequeue_task(pqueue)) != NULL) {
        if (can_acquire_resources(task) == 0) {
            printk(KERN_INFO "process_pqueue(): Task %d can acquire resources\n", task->tid);
            acquire_resources(task);
            execute_task(task);
            release_resources(task);
            kfree(task);
        } else {
            printk(KERN_INFO "process_pqueue(): Task %d cannot acquire resources\n", task->tid);
            enqueue_task(waiting_queue, task);
        }
    }
}

int process_waiting_queue() {
    int tasks_are_old = 1;
    int* ids = NULL;
    int count = 0;
    int i;
    task_t* curr = NULL;

    if (waiting_queue->head == NULL) {
        // No tasks in the waiting queue.
        return tasks_are_old;
    }

    ids = kcalloc(100, sizeof(int), GFP_KERNEL); // IDs of waiting tasks that can now acquire resources.
    count = 0; // Number of tasks that can acquire resources.

    curr = waiting_queue->head;
    while (curr != NULL) {
        if (can_acquire_resources(curr) == 0) {
            // Task can acquire resources. add its ID to the array.
            ids[count] = curr->tid;
            count++;
        }
        curr->age++;
        if (curr->age > 5) {
            // EXTRA CREDIT: Task has been waiting for 5 passes, increase priority.
            increase_priority(curr);
        }
        if (curr->age < TASK_OLD_AGE) {
            tasks_are_old = false;
        }
        curr = curr->next;
    }

    if (count == 0) {
        // No tasks can acquire resources, move on.
        kfree(ids);
        return tasks_are_old;
    }

    // Add the waiting tasks that can acquire resources to appropriate priority queues.
    mutex_lock(&pqueues_lock);
    for (i = 0; i < count; i++) {
        task_t* task = remove_task(waiting_queue, ids[i]);
        task->age = 0;
        to_pqueues(task);
    }
    mutex_unlock(&pqueues_lock); // Unlock the mutex.
    kfree(ids);
    return tasks_are_old;
}



int THREAD_schedule_tasks(void* arg) {
    int stop_scheduler = 0;
    while (are_there_any_uncompleted_tasks_left() || are_there_any_waiting_tasks_left()) {
        mutex_lock(&pqueues_lock);
        mutex_lock(&resources_lock);
        mutex_lock(&waiting_queue_lock);
        process_pqueue(pqueues->high);
        process_pqueue(pqueues->medium);
        process_pqueue(pqueues->low);
        mutex_unlock(&pqueues_lock);
        mutex_unlock(&waiting_queue_lock);
        
        mutex_lock(&waiting_queue_lock);
        stop_scheduler = process_waiting_queue();
        mutex_unlock(&resources_lock);

        if (!are_there_any_uncompleted_tasks_left() && stop_scheduler) {
            // The only remaining tasks are old and can't acquire resources, stop the scheduler.
            printk(KERN_INFO "All tasks are old and can't acquire resources, stopping scheduler\n");
            mutex_unlock(&waiting_queue_lock);
            break;
        }
        mutex_unlock(&waiting_queue_lock);
    }
    return 0;
}

void start_scheduler() {
    int i;

    // Start each consumer thread
    for (i = 0; i < NUM_CONSUMERS; i++) {
        thread_pool[i] = kthread_run(THREAD_schedule_tasks, NULL, "thread #%d", i+1);
        
    }

    // Stop each consumer thread
    for (i = 0; i < NUM_CONSUMERS; i++) {
        kthread_stop(thread_pool[i]);
    }
}

