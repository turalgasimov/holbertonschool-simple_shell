#include "shell.h"

/**
 * read_line - Reads one line from stdin byte by byte using read()
 * @line: Pointer to buffer pointer (allocated with malloc, caller must free)
 *
 * Return: number of bytes read, 0 on EOF, -1 on error
 */
ssize_t	read_line(char **line)
{
	char *buf;
	char c;
	ssize_t n;
	size_t i;

	buf = malloc(BUFSIZE);
	if (!buf)
		return (-1);
	i = 0;
	while (i < BUFSIZE - 1)
	{
		n = read(STDIN_FILENO, &c, 1);
		if (n == 0)
			break;
		if (n == -1)
		{
			free(buf);
			return (-1);
		}
		buf[i++] = c;
		if (c == '\n')
			break;
	}
	buf[i] = '\0';
	*line = buf;
	return (i);
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
 * execute_command - Forks and executes command with arguments
 * @argv: Array of arguments (argv[0] is command)
 *
 * Return: Exit status of child process, or -1 on failure
 */
int execute_command(char **argv)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			fprintf(stderr,
				"./shell: No such file or directory\n");
			exit(EXIT_FAILURE);
		}
	}

	waitpid(pid, &status, 0);
	return (status);
}

/**
 * tokenize - splits input into arguments
 * @line: input string
 *
 * Return: array of arguments
 */
char **tokenize(char *line)
{
	char **argv;
	char *token;
	int i;

	argv = malloc(sizeof(char *) * 64);
	if (argv == NULL)
		return (NULL);

	token = strtok(line, " ");
	i = 0;

	while (token != NULL)
	{
		argv[i] = token;
		i++;
		token = strtok(NULL, " ");
	}

	argv[i] = NULL;

	return (argv);
}

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0
 */
int	main(void)
{
	char *line;
	ssize_t	nread;
	int is_tty;
	int status;
	char **argv;

	is_tty = isatty(STDIN_FILENO);
	while (1)
	{
		display_prompt(is_tty);
		line = NULL;
		nread = read_line(&line);
		if (nread <= 0)
		{
			if (is_tty)
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			break;
		}
		strip_whitespace(line);
		if (line[0] != '\0')
		{
			argv = tokenize(line);
			if (argv != NULL)
			{
				status = execute_command(argv);
				free(argv);
				(void)status;
			}
		}
		free(line);
	}
	return (0);
}
