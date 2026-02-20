#include "shell.h"

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
 * strip_newline - Strips trailing whitespace (spaces, tabs, \r, \n)
 *                from a string in place
 * @str: The string to modify
 *
 * Return: void
 */
void strip_newline(char *str)
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
		close(STDIN_FILENO);
		if (execve(cmd, argv, environ) == -1)
		{
			fprintf(stderr, "./shell: No such file or directory\n");
			exit(EXIT_FAILURE);
		}
	}
	waitpid(pid, &status, 0);
	return (status);
}

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0
 */
int	main(void)
{
	char	*line;
	ssize_t	nread;
	int	is_tty;
	int	status;

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
		strip_newline(line);
		if (line[0] != '\0')
		{
			status = execute_command(line);
			(void)status;
		}
		free(line);
	}
	return (0);
}
