#ifndef RESOURCE_H
#define RESOURCE_H

#include <semaphore.h>

typedef struct resource {
    int rid;
    sem_t* sem;
    struct resource* next;
} resource_t;

#ifdef __cplusplus
extern "C" {
#endif

extern resource_t* create_resource(int rid, sem_t* sem);

extern sem_t* create_semaphore(int quantity);

#ifdef __cplusplus
}
#endif

#endif
