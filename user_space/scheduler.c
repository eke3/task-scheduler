#include "scheduler.h"

#include <stdio.h>
#include <stdlib.h>

#include "environment.h"

int run_scheduler(void) {

    printf("running scheduler\n");
    printf("there are %s pending tasks\n", (has_pending_tasks()) == 1 ? "" : "no");

    while (has_pending_tasks() == 1) {
        
        break;

    }

    if (has_pending_tasks() == FAILURE) {
        return FAILURE;
    }
    return 0;
}