#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "($) "
#define BUFSIZE 1024

extern char **environ;

void display_prompt(int is_tty);
char *read_command(char **line, size_t *len);
void strip_newline(char *str);
int execute_command(char *cmd);

/**
 * display_prompt - Prints the shell prompt if stdin is a terminal
 * @is_tty: Non-zero if stdin is a terminal, zero otherwise
 *
 * Return: void
 */
void display_prompt(int is_tty)
{
	if (is_tty)
		write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

/**
 * read_command - Reads a line of input from stdin using getline
 * @line: Address of the buffer pointer (allocated/reallocated by getline)
 * @len: Address of the buffer size variable
 *
 * Return: Pointer to the line buffer on success, NULL on EOF or error
 */
char	*read_command(char **line, size_t *len)
{
	ssize_t	nread;

	nread = getline(line, len, stdin);
	if (nread == -1)
		return (NULL);
	return (*line);
}

/**
 * strip_newline - Strips trailing whitespace (spaces, tabs, \r, \n)
 *                from a string in place
 * @str: The string to modify
 *
 * Return: void
 */
void	strip_newline(char *str)
{
	int len;

	len = strlen(str);
	while (len > 0 &&
		(str[len - 1] == '\n' || str[len - 1] == '\r' ||
		str[len - 1] == ' '  || str[len - 1] == '\t'))
	{
		str[len - 1] = '\0';
		len--;
	}
}

/**
 * execute_command - Forks and executes the given command using execve
 * @cmd: The command (full path) to execute
 *
 * Return: Exit status of the child process, or -1 on fork failure
 */
int execute_command(char *cmd)
{
	pid_t pid;
	int status;
	char *argv[2];

	argv[0] = cmd;
	argv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (execve(cmd, argv, environ) == -1)
		{
			fprintf(stderr, "./shell: No such file or directory\n");
			exit(EXIT_FAILURE);
		}
	}
	waitpid(pid, &status, 0);
	return (status);
}

#endif
