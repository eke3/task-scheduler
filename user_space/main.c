#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "environment.h"


int main() {
    // set up
    if (set_up() == FAILURE) {
        return FAILURE;
    }

    // run scheduler
    run_scheduler();

    // tear down
    return tear_down();
}