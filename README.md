# **Minez Documentation**

**Minez** is an esoteric programming language created by *Dooblix* in 2024 (current version: 2025), designed to be a more accessible and powerful version of *Brainfuck*.

---

## Installation

Download or clone the Minez repository to your desired location.

### Windows

1. Open a terminal (e.g., PowerShell or Command Prompt).
2. Navigate to the Minez installation directory:

   ```powershell
   cd "C:\Path\to\Minez"
   ```

   *Replace `C:\Path\to\Minez` with your actual installation path.*
3. Run the script to add Minez to your system PATH:

   ```powershell
   .\add_to_path.bat
   ```

   Alternatively, you can double-click the `add_to_path.bat` file.

After completion, the `minez` command will be available in any terminal.

---

### Linux / macOS

1. Open a terminal and check your shell:

   ```bash
   echo $SHELL
   ```
2. Add the Minez directory to your PATH, depending on your shell:

   **Bash**:

   ```bash
   echo 'export PATH="$PATH:/path/to/minez"' >> ~/.bashrc
   source ~/.bashrc
   ```

   **Zsh**:

   ```bash
   echo 'export PATH="$PATH:/path/to/minez"' >> ~/.zshrc
   source ~/.zshrc
   ```

   *Replace `/path/to/minez` with the actual installation path.*

Once completed, the `minez` command will be available globally.

---

## Usage

   ```
   minez <file>
   ```

   * `<file>`: Name of the `.minez` file to execute.

**Optional Arguments**

   * `--hide-input-prompts`
     Disables all default input prompts (`Int:` and `Char:`).

   * `--num-of-regs <x>`
     Sets the number of available memory cells to `x`.

   * `--docs`
     Prints this documentation file.

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
| `_`     | Pop last value from the stack into the current register (additively). |

### Register Arithmetic

| Command | Description                        |
| ------- | ---------------------------------- |
| `+y`    | Increment current register by `y`. |
| `+`     | Increment current register by 1.   |
| `-y`    | Decrement current register by `y`. |
| `-`     | Decrement current register by 1.   |
| `x`     | Reset current register to 0.       |

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
| `\|`     | Save current pointer index (`index_memory`). |
| `->`    | Jump to last saved index.                    |
| `->(y)` | Jump to the `y`-th saved index.              |
| `X`     | Delete last saved index.                     |

### Debugging

| Command | Description                                                                |
| ------- | -------------------------------------------------------------------------- |
| `d`     | Print debugging information (Memory, Pointer, Stack, Index memory, Usage). |

---
