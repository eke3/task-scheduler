#include "resource.h"

#include <stdlib.h>

resource_t* create_resource(int rid, sem_t* sem) {
    if (rid <= 0 || sem == NULL) {
        return NULL;
    }

    resource_t* resource;
    if ((resource = malloc(sizeof(resource_t))) == NULL) {
        return NULL;
    }

    resource->rid = rid;
    resource->sem = sem;
    resource->next = NULL;
    return resource;
}

sem_t* create_semaphore(int quantity) {
    sem_t* sem;
    sem = malloc(sizeof(sem_t));
    sem_init(sem, 0, quantity);
    return sem;
}
