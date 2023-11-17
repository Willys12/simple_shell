#include "main.h"

void interactive_mode(void);
void batch_mode(FILE *file);
int main(int argc, char **argv);

/**
* execute_command - excutes commands.
* @command: Command being executed.
* Return: Nothing
*/
void execute_command(char *command)
{
pid_t pid, wpid;
int status;
char *args[3];
char arg;
int i = 0;
int pipes[2];
char *token;
char *command_copy;

pid = fork();

if (pid == 0)
{
/* Child process */

command_copy = strdup(command); /* Make a copy of the command for strtok */

/* Use strtok to separate commands at pipes */
token = strtok(command_copy, "|");

pipe(pipes);
while (token != NULL)
{
pid_t sub_pid = fork();

if (sub_pid == 0)
{
/* Sub-process */

dup2(pipes[1], STDOUT_FILENO);

/* Close unused pipe ends */
close(pipes[0]);
close(pipes[1]);

/* Split the command into the command name and its arguments */

arg = strtok(token, " "); /* Split the command at spaces */
while (arg != NULL)
{
args[i++] = arg;
arg = strtok(NULL, " ");
}
args[i] = NULL; /* Null-terminate the array */

/* Execute the command */
execve(args[0], args, NULL);
perror("hsh");
_exit(EXIT_FAILURE);
}
else if (sub_pid > 0)
{
/* Wait for the sub-process to finish */
waitpid(sub_pid, NULL, 0);

/* Move to the next command in the pipeline */
token = strtok(NULL, "|");
}
else
{
perror("hsh");
_exit(EXIT_FAILURE);
}
}

/* Close the write end of the pipe in the parent */
close(pipes[1]);
/* Wait for all sub-processes to finish */
while ((wpid = wait(&status)) > 0)

free(command_copy);

_exit(EXIT_SUCCESS);
}
else if (pid < 0)
{
perror("hsh");
}
else
{
/* Parent process */
do {
wpid = waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
}

/**
* interactive_mode - checks for interactive mode.
* Return: Nothing.
*/
void interactive_mode(void)
{
char *line = NULL;
size_t len = 0;

while (1)
{
write(STDOUT_FILENO, "($) ", 4);
if (getline(&line, &len, stdin) == -1)
{
/* End of file or error */
break;
}

/* Remove newline character */
line[strcspn(line, "\n")] = '\0';

if (strcmp(line, "exit") == 0)
{
break;
}

execute_command(line);
}

free(line);
}

/**
* batch_mode - Executes a series of commands from a file in batch mode.
* @file: The file containing the commands to be executed.
* Return: Nothing.
*/
void batch_mode(FILE *file)
{
char *line = NULL;
size_t len = 0;

while (getline(&line, &len, file) != -1)
{
/* Remove newline character */
line[strcspn(line, "\n")] = '\0';

execute_command(line);
}

free(line);
}

/**
* main - Entry point.
* @argc: Number of commandline arguments.
* @argv: commandline argument vectors.
* Return: On success 0
*/
int main(int argc, char **argv)
{
int fd;

if (argc == 1)
{
/* Interactive mode */
interactive_mode();
}
else if (argc == 2)
{
/* Batch mode */
fd = open(argv[1], O_RDONLY);
if (fd == -1)
{
perror("hsh");
_exit(EXIT_FAILURE);
}

dup2(fd, STDIN_FILENO);
close(fd);

batch_mode(stdin);
}
else
{
write(STDERR_FILENO, "Usage: hsh [batch_file]\n", 24);
_exit(EXIT_FAILURE);
}

return (0);
}

