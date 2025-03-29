#include "kutils.h"


#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>


extern resource_queue_t* resources;
extern priority_queues_t* pqueues;
extern task_queue_t* waiting_queue;


void acquire_resources(task_t* task) {
   int i, j;
   int rid = 0;
   int num_needed = 0;
   resource_t* resource = NULL;


   if (task != NULL) {
       for (i = 0; i < (task->num_resources*2); i+=2) {
           rid = task->resources[i];
           num_needed = task->resources[i+1];
           resource = find_resource_id(resources, rid);
           if (resource != NULL) {
               for (j = 0; j < num_needed; j++) {
                   down(&resource->sem);
                   resource->quantity--;
               }       
           } else {
               // Resource not found.
               printk(KERN_ERR "acquire_resources() failed: Resource not found.\n");
           }
       }
   }
}


int are_there_any_uncompleted_tasks_left() {
   // Check if there are any tasks in the priority queues.
   if (pqueues != NULL) {
       if (pqueues->high != NULL) {
           if (pqueues->high->head != NULL) {
               return 1;
           }
       }
       if (pqueues->medium != NULL) {
           if (pqueues->medium->head != NULL) {
               return 1;
           }
       }
       if (pqueues->low != NULL) {
           if (pqueues->low->head != NULL) {
               return 1;
           }
       }
   }
   return 0;
}


int are_there_any_waiting_tasks_left() {
   // Check if there are any tasks in the waiting queue.
   if (waiting_queue != NULL) {
       if (waiting_queue->head != NULL) {
           return 1;
       }
   }
   return 0;
}


int can_acquire_resources(task_t* task) {
   int can_acquire_success = -1;
   int i;
   int rid = 0;
   int num_needed = 0;
   int num_available = 0;
   resource_t* resource = NULL;


   if (task != NULL) {
       can_acquire_success = 0;
       if (task->resources != NULL) {
           printk(KERN_INFO "task->resources not NULL\n");
           for (i = 0; i < (task->num_resources*2); i+=2) {
               rid = task->resources[i];
               num_needed = task->resources[i+1];
               if ((resource = find_resource_id(resources, rid)) != NULL) {
                   printk(KERN_INFO "can_acquire_resources(): Found resource %d\n", rid);
                   num_available = resource->quantity;
                   if (num_available < num_needed) {
                       // Not enough of some resource
                       printk(KERN_INFO "can_acquire_resources(): Not enough (%d) of resource %d, neeeded %d\n", num_available, rid, num_needed);
                       can_acquire_success = -1;
                       break;
                   }
               } else {
                   printk(KERN_INFO "can_acquire_resources(): Didn't find resource %d\n", rid);
                   // Didn't find a required resource, task won't be able to run.
                   can_acquire_success = -1;
                   break;
               }
           }
       }
   }
   printk(KERN_INFO "Can acquire resources?: %d\n", can_acquire_success);
   return can_acquire_success;
}






void print_rqueue(resource_queue_t* rqueue) {
   if (rqueue != NULL) {
       resource_t* curr = rqueue->head;
       printk(KERN_INFO "Printing resource queue...\n{\n");
       while (curr != NULL) {
           int quantity;
           quantity = curr->quantity;
           printk(KERN_INFO "\tResource ID: %d\tQuantity: %d\n", curr->rid, quantity);
           curr = curr->next;
       }
       printk(KERN_INFO "}\n");
   }
}


void print_tqueue(task_queue_t* tqueue) {
   if (tqueue) {
       task_t* curr = tqueue->head;
       printk(KERN_INFO "{\n");
       while (curr != NULL) {
           printk(KERN_INFO "\tTask ID: %d\n", curr->tid);
           curr = curr->next;
       }
       printk(KERN_INFO "}\n");
   }
}


void print_pqueues(priority_queues_t* pqueues) {
   if (pqueues != NULL) {
       printk(KERN_INFO "Printing priority queues...\n");
       printk(KERN_INFO "High:\n");
       print_tqueue(pqueues->high);
       printk(KERN_INFO "Medium:\n");
       print_tqueue(pqueues->medium);
       printk(KERN_INFO "Low:\n");
       print_tqueue(pqueues->low);
   }
}




void release_resources(task_t* task) {
   int i, j;
   int rid = 0;
   int num_to_release = 0;
   resource_t* resource = NULL;


   if (task != NULL) {
       for (i = 0; i < (task->num_resources*2); i+=2) {
           rid = task->resources[i];
           num_to_release = task->resources[i+1];
           resource = find_resource_id(resources, rid);
           if (resource != NULL) {
               for (j = 0; j < num_to_release; j++) {
                   up(&resource->sem);
                   resource->quantity++;
               }
           }
       }
   }
}


void to_pqueues(task_t* task) {
   if (task != NULL) {
       switch (task->priority) {
           case HIGH:
               enqueue_task(pqueues->high, task);
               break;
           case MEDIUM:
               enqueue_task(pqueues->medium, task);
               break;
           case LOW:
               enqueue_task(pqueues->low, task);
               break;
       }
   }
}



