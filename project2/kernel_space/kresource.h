#ifndef KRESOURCE_H
#define KRESOURCE_H

#include <linux/semaphore.h>

typedef struct resource {
    int rid;
    struct semaphore sem;
    int quantity;
    struct resource* next;
} resource_t;

// resource_t* create_resource(int rid, int quantity)
// Description: Creates a new resource.
// Preconditions: Valid resource parameters are passed as arguments.
// Postconditions: A new resource is created.
// Returns: A pointer to the new resource.
extern resource_t* create_resource(int rid, int quantity);

#endif
