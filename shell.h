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
ssize_t read_line(char **line);
void strip_whitespace(char *str);
int execute_command(char **argv);
char **tokenize(char *line);
char *get_path_from_environ(void);
char *find_command_in_path(char *cmd);

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
 * strip_whitespace - Strips trailing/leadingc whitespace
 * (spaces, tabs, \r, \n)
 * from a string in place
 * @str: The string to modify
 *
 * Return: void
 */
void strip_whitespace(char *str)
{
	int start = 0;
	int end = strlen(str) - 1;
	int i;

	/* Trim leading spaces */
	while (str[start] == ' ' || str[start] == '\t')
		start++;

	/* Trim trailing spaces/newlines */
	while (end >= start &&
		(str[end] == '\n' || str[end] == '\r' ||
		str[end] == ' '  || str[end] == '\t'))
	{
		str[end] = '\0';
		end--;
	}

	/* Shift string left if needed */
	if (start > 0)
	{
		i = 0;
		while (str[start])
			str[i++] = str[start++];
		str[i] = '\0';
	}
}

/**
 * get_path_from_environ - returns a pointer to the PATH string from environ
 *
 * Return: pointer to PATH string, or NULL if not found
 */
char *get_path_from_environ(void)
{
	int i;
	size_t len;

	if (!environ)
		return (NULL);

	for (i = 0; environ[i]; i++)
	{
		len = strlen("PATH=");
		if (strncmp(environ[i], "PATH=", len) == 0)
			return (environ[i] + len);
	}
	return (NULL);
}

/**
 * find_command_in_path - searches PATH for the executable
 * @cmd: command name (e.g., "ls")
 *
 * Return: full path if found (malloc'd), NULL otherwise
 */
char *find_command_in_path(char *cmd)
{
	char *path_env, *path_dup, *dir;
	char full_path[1024];

	if (!cmd || strchr(cmd, '/'))
		return (NULL); /* Already a path, don't search PATH */

	path_env = get_path_from_environ();
	if (!path_env)
		return (NULL);

	path_dup = strdup(path_env);
	if (!path_dup)
		return (NULL);

	dir = strtok(path_dup, ":");
	while (dir)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_dup);
			return (strdup(full_path));
		}
		dir = strtok(NULL, ":");
	}

	free(path_dup);
	return (NULL);
}

#endif
