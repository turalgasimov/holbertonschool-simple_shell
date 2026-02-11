# Simple Shell

## Description

Simple Shell is a UNIX command line interpreter written in C.
It replicates the basic functionality of `/bin/sh`.

The shell reads user input, parses commands, creates child processes,
and executes programs using the `execve` system call.

This project was built as part of the Holberton School curriculum
to understand how shells work internally.

---

## Features

- Interactive mode
- Non-interactive mode
- Executes commands with absolute paths
- Handles errors like `/bin/sh`
- Uses fork, execve, and wait system calls
- No memory leaks

---

## Compilation

Compiled on Ubuntu 20.04 LTS using:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

---

## Usage

### Interactive mode:
```bash
$ ./hsh
($) /bin/ls
file1 file2 main.c
($) exit
$
```

### Non-interactive mode:
```bash
$ echo "/bin/ls" | ./hsh
file1 file2 main.c
```

---

by Tural Gasimov and Amin Israfilov