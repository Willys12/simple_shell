#include "main.h"
/* Working prototypes */
void command_Execute(char *command);
void reverse(char s[]);
void itoa(int n, char s[]);

/***/
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void command_Execute(char *command)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        _exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /* Child process */

        /* Execute the command */
        if (execlp(command, command, (char *)NULL) == -1)
        {
            perror(command);
            _exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */

        /* Wait for the child process to finish */
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            char exit_status[4];
            itoa(WEXITSTATUS(status), exit_status);
            write(STDERR_FILENO, "Error: Command failed with exit status ", 40);
            write(STDERR_FILENO, exit_status, strlen(exit_status));
            write(STDERR_FILENO, "\n", 1);
        }
    }
}

/**
* main - Entry point.
* Return: 0 on success.
*/
int main(void)
{
    char *command = NULL;
    size_t command_len = 0; /* Initialize to zero */
    ssize_t read;
	size_t len;

    while (1)
    {
        /* Shell prompt */
        write(STDOUT_FILENO, "$ ", 2);

        /* Checking inputs */
        read = getline(&command, &command_len, stdin);

        if (read == -1)
        {
            /* Shell exit on EOF (Ctrl+D) */
            write(STDOUT_FILENO, "\nExiting shell...\n", 18);
            break;
        }

        /* Remove newline character if present */
        len = strlen(command);
        if (len > 0 && command[len - 1] == '\n')
        {
            command[len - 1] = '\0';
        }

        /* Execution is here */
        command_Execute(command);
    }

    /* After the loop, free the dynamically allocated memory */
    free(command);

    return 0;
}

