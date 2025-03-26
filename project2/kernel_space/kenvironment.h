#ifndef KENVIRONMENT_H 
#define KENVIRONMENT_H

#include "kutils.h"

// void set_up()
// Description: Sets up the environment for the scheduler.
// Preconditions: None.
// Postconditions: The environment is set up and global variables are initialized.
// Returns: None.
extern void set_up(void);

// void tear_down()
// Description: Tears down the environment for the scheduler.
// Preconditions: Global variables are initialized.
// Postconditions: The environment is torn down and global variables are deallocated.
// Returns: None.
extern void tear_down(void);

#endif