#include "kresource.h"

#include <linux/slab.h>

resource_t* create_resource(int rid, int quantity) {
    resource_t* resource;

    if (rid <= 0 || quantity < 0) {
        // Don't make resources with IDs less than 1.
        return NULL;
    }

    if ((resource = (resource_t*)kmalloc(sizeof(resource_t), GFP_KERNEL)) == NULL) {
        // Memory allocation failed.
        return NULL;
    }

    resource->rid = rid;
    sema_init(&resource->sem, quantity);
    resource->quantity = quantity;
    resource->next = NULL;
    return resource;
}