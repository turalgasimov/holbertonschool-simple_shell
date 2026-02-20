#include "shell.h"

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0
 */
int main(void)
{
	char	*line;
	size_t	len;
	int	is_tty;
	int	status;

	line = NULL;
	len = 0;
	is_tty = isatty(STDIN_FILENO);
	while (1)
	{
		display_prompt(is_tty);
		if (read_command(&line, &len) == NULL)
		{
			if (is_tty)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		strip_newline(line);
		if (line[0] == '\0')
			continue;
		status = execute_command(line);
		(void)status;
	}
	free(line);
	return (0);
}
