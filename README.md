# Minez Documentation

**Minez** is an esoteric programming language created by *Dooblix* in 2024 (current version: 2025), inspired by Urban Müller's minimalistic esoteric programming language "*Brainfuck*".

---

## Installation

Pre-compiled binaries are available for Windows and Linux. You can find them in the `bin/` directory or download them from the GitHub Releases section. For those who prefer building from source, follow the steps below.

---

### 1. Prerequisites

- **C compiler installed**:
  - **Windows:** MinGW-w64 or MSYS2 (ensure `gcc` is in your PATH)
  - **Linux/macOS:** `gcc` or `clang` (e.g., `sudo apt install build-essential` on Ubuntu/Debian)
- **Git** (optional, for cloning the repository)
- Terminal / command prompt with access to `gcc`

---

### 2. Download the repository

**Option 1: Clone**

```bash
git clone https://github.com/Dooblix/Minez.git
cd Minez
```

**Option 2: Download ZIP**

* Download the ZIP from GitHub and extract it.
* Navigate to the extracted folder:

```bash
cd Minez
```

> Remove any existing binaries in `bin/` before building from source to avoid conflicts.

---

### 3. Compile

**Windows:**

```bash
gcc -O3 src/*.c -o bin/minez-windows.exe
```

**Linux/macOS:**

```bash
gcc -O3 src/*.c -o bin/minez-linux -lm
```

* `-O3` enables optimization.
* Optional: `-Wall` enables all compiler warnings.
* For maximum portability across Linux systems, you can use static linking:

```bash
gcc -O3 src/*.c -o bin/minez-linux-static -lm -static
```

---

### 4. Test

```bash
# Windows
bin\minez-windows.exe --help

# Linux/macOS
./bin/minez-linux --help
```

If the help message appears, the interpreter has been successfully compiled.

---

## Usage

Run a Minez program with:

```bash
minez <path-to-program> [-h] [--help] [-q] [--quiet] [--num-of-regs N] [--print-until N] [--print-intervals A B ...] [--pre-input ""] [--no-pause]
```

Options:

* `--num-of-regs N` — Number of registers to allocate (default: 100).
* `--print-until N` — Number of memory cells to display after program ends and when using `d` (default: num_of_regs).
* `--print-intervals A B ...` — Print memory contents in specified intervals. Each interval is a pair of indices `A B`. Multiple intervals can be provided. Must have an even number of integers.
* `--pre-input ""` — Provides input values before execution. Separate integers with a delimiter (newline recommended). The number of inputs must match program expectations. Example:

```bash
.\bin\minez-windows.exe examples/official/calc.minez --pre-input "234\n-34\n"
```

* `--no-pause` — Do not wait for keypress after program ends.
* `--quiet`/`-q` — Disables all printing of metadata and debugger (`d`) commands.
* `--help`/`-h` — Prints out this usage guide.

Example:

```bash
./bin/minez-windows.exe examples/official/hello_world.minez --num-of-regs 1
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

`minez_helper.py` can generate repetitive Minez code automatically (printing, strings or lists):

```bash
python scripts/minez_helper.py string "Hello" --reg 1 # >1+72#+29#+7##+3# 
python scripts/minez_helper.py list "10,20,-30" --reg 5 --clear-garbage # >5x+10>x+20>x-30
```

* `--clear-garbage` — Resets used registers to `0`.
* `--as-list` — Store a string as a list of ASCII values (only for type `string`).
* **Note:** `string` (without `--as-list`) produces a Minez snippet that prints the string.

---

## Syntax Reference
**Note:** This is a simplified summary. Refer to `spec.md` for the full specification.

### Pointer Manipulation

| Command | Description                         |
| ------- | ----------------------------------- |
| `>y`    | Move pointer to absolute index `y`. |
| `>`     | Increment pointer by 1.             |
| `<`     | Decrement pointer by 1.             |

### Flow Control

| Command | Description                                                                                   |
| ------- | --------------------------------------------------------------------------------------------- |
| `^y`    | Jump to program index `y`, saving pointer of the next command on the stack.                   |
| `^s`    | Jump back to the last saved value on the stack.                                               |
| `[`     | Begin loop (executes while current register ≠ 0). Skipped if register = 0.                    |
| `~`     | Skip to next loop iteration (like `continue`).                                                |
| `]`     | End loop (repeats if register ≠ 0).                                                           |
| `{}(...)` | Conditional block: `{reg>reg}`, `{reg<reg}`, `{reg=reg}`. Use `i` for the current register. |
| `;`     | End program.                                                                                  |

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
| `d`     | Print debugging information (Memory, Pointer, Stack, Index memory, Loop stack). |

---

## Examples

This repository contains two kinds of examples:

* `examples/official/` — written and maintained by me, thoroughly tested, and meant to demonstrate how Minez works and what it can do
* `examples/community/` — written by the community, showcasing what people build with Minez

Got a cool Minez project?
Open a GitHub Issue and share it — It might be added it to `examples/community/`!

---

## Author

Dooblix © 2026
