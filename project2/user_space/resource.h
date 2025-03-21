#ifndef RESOURCE_H
#define RESOURCE_H

#include <semaphore.h>

// Struct for holding a resource's information.
typedef struct resource {
    int rid;
    // Semaphore for holding the quantity of the resource.
    sem_t* sem;
    struct resource* next;
} resource_t;

#ifdef __cplusplus
extern "C" {
#endif

// resource_t* create_resource(int rid, sem_t* sem)
// Description: Creates a new resource.
// Preconditions: Valid resource parameters are passed as arguments.
// Postconditions: A new resource is created.
// Returns: A pointer to the new resource.
extern resource_t* create_resource(int rid, sem_t* sem);

// sem_t* create_semaphore(int quantity)
// Description: Creates a new semaphore.
// Preconditions: Valid semaphore parameters are passed as arguments.
// Postconditions: A new semaphore is created.
// Returns: A pointer to the new semaphore.
extern sem_t* create_semaphore(int quantity);

#ifdef __cplusplus
}
#endif

#endif
