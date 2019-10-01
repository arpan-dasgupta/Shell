# Shell (ASH)

## Aim

- To build a shell performing basic operations.
- Must have cd, ls, pwd and echo inbuilt.
- Other commands must be implemented using exec.

## Constraints

- No piping implemented yet.
- Some unhandled corner cases.

## Usage

- Run command
  `make;make clean;./ash` to build from source
  or simply
  `./ash`
  on terminal

## Working

- **_shell.c_** - main loop of the shell, output prompt and input command.
- **_command.c_** - all form of string parsing, command processing and call to respective functions is performed here.
- **_init.c_** - initializes prompt.
- **_list.c_** - implements ls and variants.
- **_pwd.c_** - implements pwd.
- **_cd.c_** - implements cd and variants.
- **_echo.c_** - implements echo.
- **_others.c_** - implements other commands using exec.
- **_history.c_** - implements history command.
- **_nightswatch.c_** - implements a specific version of watch command.
- **_pinfo.c_** - implements pinfo command.
- **_setenv.c_** - implements setenv command.
- **_unsetenv.c_** - implements unsetenv command.
