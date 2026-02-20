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
char *read_line(char **line);
void strip_newline(char *str);
int execute_command(char *cmd);

#endif
