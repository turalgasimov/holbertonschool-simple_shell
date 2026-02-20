#include "shell.h"

/**
 * main - simple shell
 * Return: Always 0
 */
int main(void)
{
	char *line = NULL;
	char *argv[2];
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int status;

	while (1)
	{
		printf("($) ");
		fflush(stdout);
		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			printf("\n");
			free(line);
			exit(0);
		}
		line[read - 1] = '\0';
		pid = fork();
		if (pid == -1)
		{
			perror("Error");
			free(line);
			exit(1);
		}
		if (pid == 0)
		{
			argv[0] = line;
			argv[1] = NULL;
			execve(line, argv, environ);
			perror("./hsh");
			exit(1);
		} else
		{
			wait(&status);
		}
	}
	free(line);
	return (0);
}
