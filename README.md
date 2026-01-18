# **Minez Documentation**

**Minez** is an esoteric programming language created by *Dooblix* in 2024 (current version: 2025), designed to be a more accessible and powerful version of Urban Müller's minimalistic esoteric programming language "*Brainfuck*".

---

## Installation

Pre-compiled binaries are provided. For those who prefer building from source, follow the steps below.

---

### 1. Prerequisites

- **C compiler installed**:
  - **Windows:** MinGW-w64 or MSYS2 (ensure `gcc` is in your PATH)
  - **Linux:** `gcc` or `clang` (e.g., `sudo apt install build-essential` on Ubuntu/Debian)
- **Git** (optional, for cloning the repository)
- Terminal / command prompt with access to `gcc`

---

### 2. Download the repository

**Option 1: Clone**

```bash
git clone https://github.com/Dooblix/Minez.git
cd minez
```

**Option 2: Download ZIP**

- Download the ZIP from GitHub and extract it.
- Navigate to the extracted folder:

```bash
cd minez
```

Remove any existing pre-compiled binaries before building from source.

---

### 3. Create build folder

```bash
mkdir -p build
```

> `-p` ensures the folder is only created if it doesn't exist (works on Linux and Windows PowerShell).

---

### 4. Compile

**Windows:**

```bash
gcc -O3 src/main.c src/minez_docs.c src/vectors.c src/interpreter.c src/utils.c -o build/minez.exe
```

**Linux / macOS:**

```bash
gcc -O3 src/main.c src/minez_docs.c src/vectors.c src/interpreter.c src/utils.c -o build/minez
```

- `-O3` enables optimization.  
- optional: `-Wall` enables all compiler warnings.

---

### 5. Test

```bash
# Windows
build\minez.exe --docs

# Linux/macOS
./build/minez --docs
```

If the documentation message appears, the interpreter has been successfully compiled.

---

## Usage

Run a Minez program with:

```bash
./minez <path-to-program> [-d] [--docs] [-q] [--quiet] [--num-of-regs N] [--print-until N] [--print-intervalls A B ...] [--pre-input ""] [--no-pause]
```

Options:

* `--num-of-regs N` — Number of registers to allocate (default: 100).  
* `--print-until N` — Number of memory cells to display after program ends and when using `d` (default: num_of_regs).  
* `--print-intervalls A B ...` — Print memory contents in specified intervals. Each interval is a pair of indices `A B`. Multiple intervals can be provided. Must have an even number of integers.
* `--pre-input ""` — Allows the user to provide input values that the program uses before execution. Each input integer must be separated by a delimiter. Any character can be used as a delimiter, but '\n' is recommended for readability. The length of the pre-input string must match the number of inputs the program expects. (Example: ```.\build\minez.exe examples\calc.minez --pre-input "234\n/-34\n"```)
* `--no-pause` — Do not wait for keypress after program ends.
* `--quiet`/`-q` — Disables all printing of metadata and debugger (`d`) commands.
* `--docs`/`-d` — Prints a shortened version of this README.

Example:

```bash
./minez.exe ../examples/hello_world.minez --num-of-regs 1
```

Will output:

```
Hello World!

-------------------------------------

Program halted at index 40.

Program ended successfully!
Memory: 33
Pointer: 0
Stack: empty
Index memory: empty
Runtime: 0.001161 sec
Press any key to continue...
```

---

## Python Helper

`minez_helper.py` can generate repetitive Minez code automatically (strings or lists):

```bash
python scripts/minez_helper.py string "Hello" --reg 1 # >1+72#+29#+7##+3# 
python scripts/minez_helper.py list "10,20,-30" --reg 5 --clear-garbage # >5x+10>x+20>x-30
```

* `--clear-garbage` — Resets used registers to `0`.
* `--as-list` — Store a string as a list of ASCII values (only for type `string`).

---

## Syntax Reference

### Pointer Manipulation

| Command | Description                         |
| ------- | ----------------------------------- |
| `>y`    | Move pointer to absolute index `y`. |
| `>`     | Increment pointer by 1.             |
| `<`     | Decrement pointer by 1.             |

### Flow Control

| Command | Description                                                                                                        |
| ------- | ------------------------------------------------------------------------------------------------------------------ |
| `^y`    | Jump to program index `y`, saving pointer of the next command on the stack.                                        |
| `^s`    | Jump back to the last saved value on the stack.                                                                    |
| `[`     | Begin loop (executes while current register ≠ 0). Skipped if register = 0.                                         |
| `~`     | Skip to next loop iteration (like `continue`).                                                                     |
| `]`     | End loop (repeats if register ≠ 0).                                                                                |
| `{}`    | Conditional block. Syntax: `{reg>reg}(...)`, `{reg<reg}(...)`, `{reg=reg}(...)`. Use `i` for the current register. |
| `;`     | End program.                                                                                                       |

### Stack Operations

| Command | Description                                                           |
| ------- | --------------------------------------------------------------------- |
| `@`     | Push value of current register onto the stack.                        |
| `@R`    | Push current runtime (in ms) onto the stack.                          |
| `_`     | Pop last value from the stack into the current register (additively). |

### Register Arithmetic

| Command | Description                                                 |
| ------- | ----------------------------------------------------------- |
| `+y`    | Increment current register by `y`.                          |
| `+R`    | Add current runtime (in ms) to the current register.        |
| `+`     | Increment current register by 1.                            |
| `-y`    | Decrement current register by `y`.                          |
| `-R`    | Subtract current runtime (in ms) from the current register. |
| `-`     | Decrement current register by 1.                            |
| `x`     | Reset current register to 0.                                |

### Input/Output

| Command | Description                                                        |
| ------- | ------------------------------------------------------------------ |
| `#`     | Output ASCII character of current register.                        |
| `#!`    | Output numeric value of current register.                          |
| `.`     | Read a single input character into current register (ASCII).       |
| `:`     | Read an integer into current register (any size, incl. negatives). |

### Index Memory

| Command | Description                                  |
| ------- | -------------------------------------------- |
| `\|`    | Save current pointer index (`index_memory`). |
| `->`    | Jump to last saved index.                    |
| `->(y)` | Jump to the `y`-th saved index.              |
| `X`     | Delete last saved index.                     |

### Debugging

| Command | Description                                                                |
| ------- | -------------------------------------------------------------------------- |
| `d`     | Print debugging information (Memory, Pointer, Stack, Index memory, Usage). |

---

## Notes

* Minez is experimental and intended for learning and experimentation.
* Large programs or deep loops may significantly increase runtime.
* Syntax errors (unmatched `]` or `)`) will terminate the program with a descriptive error message.

---

## Author

Dooblix © 2025
