#ifndef MAIN_H
#define MAIN_H

/*All the working standard libraries*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024

/* All the code prototypes and constants goes here */
extern char **environ; /* External enviroment variable */

/*These are the working prototypes*/
void command_Execute(char *command);
#endif
