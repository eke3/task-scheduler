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

void test_sys_add_task(void);
void test_sys_schedule_tasks(void);

int main() {

    test_sys_add_task();



    return 0;
}

void test_sys_add_task() {
    set_up_scheduler_syscall();

    int tid = 420;
    task_priority_t priority = HIGH;
    int duration = 1;
    int* resources = calloc(2,sizeof(int));
    resources[0] = 1;
    resources[1] = 1;
    size_t num_resources = 1;
    add_task_syscall(tid,priority, duration, resources, num_resources);
    print_pqueues_syscall();
    schedule_tasks_syscall();
    print_pqueues_syscall();


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