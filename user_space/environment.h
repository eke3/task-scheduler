#ifndef ENVIRONMENT_H 
#define ENVIRONMENT_H

#define INITIALIZED 1
#define NOT_INITIALIZED 0

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void set_up(void);

extern void tear_down(void);

#ifdef __cplusplus
}
#endif

#endif