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

void test_sys_add_task(void);
void test_sys_schedule_tasks(void);

int main() {

    test_sys_add_task();



    return 0;
}

void test_sys_add_task() {
    set_up_scheduler_syscall();

    // long rv;
    // task_t* task;
    // task = create_task(1, HIGH, 1, NULL, 0);


    // rv = add_task_syscall(task);


    // if(rv < 0) {
    //     perror("add task syscall failed");
    // }
    // else {
    //     printf("add task syscall ran successfully, check dmesg output\n");
    // }


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