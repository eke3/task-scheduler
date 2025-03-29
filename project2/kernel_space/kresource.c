#include "kresource.h"

#include <linux/slab.h>
#include <linux/kernel.h>

resource_t* create_resource(int rid, int quantity) {
    printk(KERN_INFO "create_resource(): Creating resource %d with quantity %d...\n", rid, quantity);
    resource_t* resource;

    if (rid <= 0 || quantity < 0) {
        // Don't make resources with IDs less than 1.
        return NULL;
    }

    if ((resource = (resource_t*)kmalloc(sizeof(resource_t), GFP_KERNEL)) == NULL) {
        // Memory allocation failed.
        printk(KERN_ERR "create_resource() failed: Memory allocation error.\n");
        return NULL;
    }

    resource->rid = rid;
    sema_init(&resource->sem, quantity);
    resource->quantity = quantity;
    printk(KERN_INFO "create_resource(): Resource %d quantity set to %d\n", rid, resource->quantity);
    resource->next = NULL;
    printk(KERN_INFO "create_resource(): Resource %d created %ssuccessfully.\n", rid, (resource == NULL) ? "un" : "");
    return resource;
}