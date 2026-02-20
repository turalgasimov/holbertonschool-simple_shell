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
 * execute_command - Forks and executes command with arguments
 * @argv: Array of arguments (argv[0] is command)
 *
 * Return: Exit status of child process, or -1 on failure
 */
int execute_command(char **argv)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!argv[0])
		return (127);

	if (strchr(argv[0], '/'))
		cmd_path = strdup(argv[0]);
	else
		cmd_path = find_command_in_path(argv[0]);

	if (!cmd_path)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("./hsh");
		free(cmd_path);
		return (127);
	}

	if (pid == 0)
	{
		if (execve(cmd_path, argv, environ) == -1)
		{
		perror("./hsh");
		exit(127);
		}
	}

	waitpid(pid, &status, 0);
	free(cmd_path);
	return (WIFEXITED(status) ? WEXITSTATUS(status) : 127);
}

/**
 * tokenize - splits input into arguments
 * @line: input string
 *
 * Return: array of arguments
 */
char **tokenize(char *line)
{
	char **argv = NULL;
	char *token;
	int size = 8, i = 0;

	argv = malloc(sizeof(char *) * size);
	if (!argv)
		return (NULL);

	token = strtok(line, " ");
	while (token != NULL)
	{
		if (i >= size - 1)
		{
			size *= 2;
			argv = realloc(argv, sizeof(char *) * size);
			if (!argv)
				return (NULL);
		}
		argv[i++] = token;
		token = strtok(NULL, " ");
	}
	argv[i] = NULL;
	return (argv);
}

/**
 * main - Entry point for the simple shell
 *
 * Return: status
 */
int main(void)
{
	char *line;
	ssize_t nread;
	int is_tty;
	int status = 0;
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
				/* Handle built-in exit */
				if (strcmp(argv[0], "exit") == 0)
				{
					free(argv);
					free(line);
					break;
				}
				status = execute_command(argv);
				free(argv);
			}
		}
		free(line);
	}
	return (status);
}
