#ifndef MAIN_H
#define MAIN_H

/*All the working standard libraries*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

/* All the code prototypes and constants goes here */
extern char **environ; /* External enviroment variable */

/*These are the working prototypes*/
void execute_command(char *command);
void interactive_mode();
void batch_mode(FILE *file);
int main(int argc, char **argv);

#endif
