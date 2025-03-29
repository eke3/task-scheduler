// #include "../kernel_space/kenvironment.h"
// #include "../kernel_space/kscheduler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include "../kernel_space/ktask_priority.h"

#define __NR_add_task 548
#define __NR_schedule_tasks 549
#define __NR_set_up_scheduler 550
#define __NR_tear_down_scheduler 551
#define __NR_print_pqueues 552
#define __NR_print_wqueue 553
#define __NR_print_rqueue 554
#define __NR_add_resource 555

long add_task_syscall(int tid, task_priority_t priority, int duration, int* resources, size_t num_resources) {
    return syscall(__NR_add_task, tid, priority, duration, resources, num_resources);
}

long schedule_tasks_syscall() {
    return syscall(__NR_schedule_tasks);
}

long set_up_scheduler_syscall() {
    return syscall(__NR_set_up_scheduler);
}  

long tear_down_scheduler_syscall() {
    return syscall(__NR_tear_down_scheduler);
}

long print_pqueues_syscall() {
    return syscall(__NR_print_pqueues);
}

long print_wqueue_syscall() {
    return syscall(__NR_print_wqueue);
}

long print_rqueue_syscall() {
    return syscall(__NR_print_rqueue);
}

long add_resource_syscall(int rid, int quantity) {
    return syscall(__NR_add_resource, rid, quantity);
}

void generate_tasks() {
    for (int i = 0; i < 10; i++) {
        int tid = rand();
        task_priority_t priority = (task_priority_t)(rand() % 3 + 1);
        int duration = (rand() % 3 + 1);
        int* resource_array = calloc(4,sizeof(int));
        resource_array[0] = (rand() % 9) + 1; // random resource numbered from 1-9
        resource_array[1] = (rand() % (5 - 0 + 1)) + 0; // random quantity between 0 and 5
        resource_array[2] = resource_array[0] + 1; // Make sure the second resource is different from the first.
        resource_array[3] = (rand() % (5 - 0 + 1)) + 0; // random quantity between 0 and 5
        size_t num_resources = 2;
        add_task_syscall(tid, priority, duration, resource_array, num_resources);
        free(resource_array);
    }
}

void generate_resources() {
    for (int i = 1; i <= 10; i++) {
        add_resource_syscall(i, i);
    }
}

void test_sys_add_task(void);
void test_sys_schedule_tasks(void);

int main() {
    set_up_scheduler_syscall();
    generate_resources();

    generate_tasks();

    schedule_tasks_syscall();
    


    // for (int i = 0; i < 3; i++) {
    //     wake_up_process(thread_pool[i]);
    // }

    // for (int i = 0; i < 3; i++) {
    //     kthread_stop(thread_pool[i]);
    // }

    print_pqueues_syscall();
    // printf("Waiting queue:\n");
    print_wqueue_syscall();
    print_rqueue_syscall();

    tear_down_scheduler_syscall();
    // test_sys_add_task();



    return 0;
}

void test_sys_add_task() {
    set_up_scheduler_syscall();

    int tid = 420;
    task_priority_t priority = HIGH;
    int duration = 1;
    int* resources = calloc(2,sizeof(int));
    resources[0] = 2;
    resources[1] = 5;
    size_t num_resources = 1;
    add_resource_syscall(1,1);
    print_rqueue_syscall();
    add_task_syscall(tid,priority, duration, resources, num_resources);
    print_pqueues_syscall();

    add_resource_syscall(2,2);
    schedule_tasks_syscall();
    print_pqueues_syscall();
    print_wqueue_syscall();
    print_rqueue_syscall();


    tear_down_scheduler_syscall();
}

void test_sys_acquire_resources() {
    // set_up();

    // tear_down();
}

void test_sys_release_resources() {
    // set_up();

    // tear_down();
}

void test_sys_schedule_tasks() {
    // set_up();

    // tear_down();
}