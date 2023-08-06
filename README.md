# <center>bojSolutionDocsGenerator</center>
a simple macro for my repository [dilluti0n/baekjun](https://github.com/dilluti0n/baekjun) which uses jekyll to run its [documentation site](https://dilluti0n.github.io/baekjun). This macro automatically merges the `assets/sol/xxxx.md` file of the repository with the `xxxx.(extension)` file in the folder named after each source code's extension (e.g. `py/`, `c/`) and uploads it to the site.

## requirements
* Your OS must be based on Unix. (e.g. Linux, macOS)
* `git` and your boj repository with following rules :
```bash
├── assets
│   ├── origin.md
│   └── sol
│       └──{problem_number}.md
└── {extension}
    └── {problem_number}.{extension}
```
1. `assets\origin.md` should contain the base of your documentation's homepage.
2. `assets\sol\{probem_number}.md` should contain a description of the algorithm of the `{problem_number}`.
3. `{extension}\{problem_number}.{extension}` should contain a source code of the algorithm whih a language `{extension}`. (e.g. `c\1000.c`, `py\1000.py`)

## how to compile & use
There are three ways to compile and use this macro.
### way I (recommended)
1. `vim ./makefile` and modify the syntex `{your_path}` to your BOJ directory path.
2. `make ucompile` to compile the macro.
3. `make run` to run the macro.
### way II
1. `make compile` to compile the macro.
2. `mv ./bojSolGen {your_path}` to move the macro to your BOJ direcrory path.
3. after move your program, `cd {your_path}` and `./bojSolGen` to run the macro.
### way III
1. `make jcompile` to compile the macro.
2. `./bojSolGen` to run the macro. (In this way, you need to type `{your_path}` in the macro.)

Note that only the first method commits the changes made by the macro to the BOJ repository and pushes them to github automatically.
