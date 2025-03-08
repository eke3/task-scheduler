#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "environment.h"


int main() {
    // set up
    if (set_up() == FAILURE) {
        tear_down();
        return EXIT_FAILURE;
    }

    // run scheduler
    if (run_scheduler() == FAILURE) {
        tear_down();
        return EXIT_FAILURE;
    }

    // tear down
    return tear_down();
}