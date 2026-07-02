# Shell (C)

A small educational Unix-like command shell implemented in C. The project demonstrates command parsing, builtins, and I/O redirection (including support for `>`, `>>`, `1>`, `1>>`, and `2>`).

## Features

- Simple interactive prompt
- Builtin commands: `pwd`, `cd`, `exit`, `type`, `echo`
- Execute external programs found via `PATH`
- Output redirection: overwrite (`>` / `1>` / `2>`) and append (`>>` / `1>>` / `2>>`)
- Basic handling of quoted arguments and escapes

## Repository layout

- `src/` — all C source files and headers (the shell implementation)

Key files in `src/`:

- `main.c` — main loop and command dispatch
- `exec.c` — external command execution and redirection handling
- `redir.c` — builtin `echo` and redirection helper logic
- `utils.c`, `utils.h` — helper utilities and parsing helpers

## Build

Preffered: compile with `cc`/`gcc` directly

```bash
cc -D_POSIX_C_SOURCE=200809L -std=c23 src/*.c -o shell_c
./codecrafters_shell
```

## Usage examples

- Run builtins:

```text
$ pwd
/home/user/project
$ cd src
$ exit
```

- Execute external commands and redirect output:

```text
$ ls -l > out.txt       # overwrite out.txt
$ echo hello >> out.txt # append to out.txt
$ /bin/echo hi 1>> out.txt
$ somecmd 2> errors.txt # redirect stderr
```

## Notes & limitations

- This is an educational/demo shell and does not implement full POSIX shell features (no job control, pipelines, complex expansions, or robust signal handling).
- Quoting and escaping aim to be helpful but are not a complete shell parser.

## Contributing

Patches, bug reports, and improvements are welcome. Please open a GitHub issue or submit a PR.

## License

MIT License — feel free to reuse and adapt.

---

This README was generated to document a shell implementation in C located in the `src/` directory.
