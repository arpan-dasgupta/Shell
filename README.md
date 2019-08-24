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

- ___shell.c___ - main loop of the shell, output prompt and input command.
- ___command.c___ - all form of string parsing, command processing and call to respective functions is performed here.
- ___init.c___ - initializes prompt.
- ___list.c___ - implements ls and variants.
- ___pwd.c___ - implements pwd.
- ___cd.c___ - implements cd and variants.
- ___echo.c___ - implements echo.
- ___others.c___ - implements other commands using exec.
- ___history.c___ - implements history command.
- ___nightswatch.c___ - implements a specific version of watch command.
- ___pinfo.c___ - implements pinfo command.
