#include "shell.h"

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0
 */
int main(void)
{
	char buf[BUFSIZE];
	char *line;
	int is_tty;
	int status;

	is_tty = isatty(STDIN_FILENO);
	while (1)
	{
		display_prompt(is_tty);
		line = read_command(buf, BUFSIZE);
		if (line == NULL)
		{
			if (is_tty)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		strip_newline(buf);
		if (buf[0] == '\0')
			continue;
		status = execute_command(buf);
		(void)status;
	}
	return (0);
}
