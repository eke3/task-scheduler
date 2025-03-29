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


void add_resources() {
   for (int i = 1; i <= 10; i++) {
       add_resource_syscall(i,i+1);
   }
}


void add_tasks() {
   int min_rsrc = 0; // Minimum number of a resource needed to run a task.
   int max_rsrc = 2; // Maximum number of a resource needed to run a task.
   task_priority_t min_priority = LOW;
   task_priority_t max_priority = HIGH;


   // Create NUM_TASKS tasks and inject them into the scheduler.
   for (int i = 1; i <= 30; i++) {


       int task_id = rand();
       task_priority_t priority = (task_priority_t)(rand() % (max_priority - min_priority + 1)) + min_priority;
       int task_duration = 1000;// (rand() % 3) + 1; // Task duration between 1 and 3 seconds.


       // Resources and quantities required for a task. ( [resource_id, quantity, resource_id, quantity, ...] )
       int* resource_array = malloc(4 * sizeof(int));
       resource_array[0] = (rand() % 9) + 1;
       resource_array[1] = (rand() % (max_rsrc - min_rsrc + 1)) + min_rsrc;
       resource_array[2] = resource_array[0] + 1; // Make sure the second resource is different from the first.
       resource_array[3] = (rand() % (max_rsrc - min_rsrc + 1)) + min_rsrc;
       size_t num_resources = 2;
       add_task_syscall(task_id,priority, task_duration, resource_array, num_resources);
   } 
}


int main() {


   set_up_scheduler_syscall();
   add_resources();
   add_tasks();
   print_pqueues_syscall();
   print_wqueue_syscall();
   print_rqueue_syscall();
   schedule_tasks_syscall();
   print_pqueues_syscall();
   print_wqueue_syscall();
   print_rqueue_syscall();
   tear_down_scheduler_syscall();




   return 0;
}








