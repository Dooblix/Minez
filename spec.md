# Specification of the Minez Virtual Machine
This document specifies the runtime behavior of the Minez Virtual Machine as implemented in `interpreter.c`.

---

## Table of Contents
0. Before Execution
1. Program Execution Model
2. Memory Model
3. Integer Semantics
4. Stacks and Auxiliary Structures
5. Instruction Set
6. Error Model
7. Runtime

---

## 0. Before Execution
* `//` comments are removed
* `;` is added to the end of the program if not already there

---

## 1. Program Execution Model
### 1.1 Program Representation

* A Minez program is a finite sequence of characters (`char` aka `uint8_t`)
* A Minez command is a one character or a set of characters + a numeric parameter sometimes

### 1.2 Program Counter (PC)

* The program counter (`pc`) is a zero-based index into the program.
* Execution starts at `pc = 0`.
* After each instruction, `pc` is incremented by 1 unless explicitly modified.
* Execution terminates when:

  * the `;` instruction is executed, or
  * a fatal error occurs.

---

## 2. Memory Model

### 2.1 Register Memory

* The VM maintains a fixed-size array of signed integers (`int`).
* The size is defined at program start (`num_of_regs`).
* All registers are initialized to `0`.

### 2.2 Pointer

* The pointer (`ptr`) references the current register.
* Pointer type is unsigned (`size_t`).
* Initial value: `0`.

### 2.3 Pointer Validity

* Any instruction that moves the pointer **must keep it within**
  `0 ≤ ptr < num_of_regs`.
* Violations cause **immediate termination** with `IndexError`.

---

## 3. Integer Semantics

* Registers store signed integers of implementation-defined width (`int`).
* Overflow behavior follows the host C implementation.

---

## 4. Stacks and Auxiliary Structures

### 4.1 Data Stack

* A LIFO stack of signed integers.
* Initially empty.

### 4.2 Loop Stack

* A LIFO stack of program counter positions.
* Used exclusively for loop control (`[` / `]` / `~`).

### 4.3 Index Memory

* An ordered list of pointer values (`size_t`).
* Used for indirect pointer jumps.

---

## 5. Instruction Set

### 5.1 Pointer Manipulation

#### `>`

* If followed by digits:

  * Parse an unsigned integer `y`
  * Set `ptr = y`
* Otherwise:

  * Increment `ptr` by 1
* If `ptr ≥ num_of_regs`: `IndexError`

#### `<`

* Decrement `ptr` by 1
* If `ptr == 0` before decrement: `IndexError`

---

### 5.2 Arithmetic

#### `+`

* `+y`: add parsed integer `y`
* `+R`: add runtime in milliseconds
* `+`: increment by 1

#### `-`

* `-y`: subtract parsed integer `y`
* `-R`: subtract runtime
* `-`: decrement by 1

#### `x`

* Set current register to `0`

---

### 5.3 Index Memory Operations

#### `|`

* Push the current pointer value onto index memory

#### `X`

* Pop last value from index memory
* If empty: `StackError`

#### `->`

* Jump pointer to last saved index
* Requires non-empty index memory

#### `->(y)`

* Jump pointer to index_memory[y]
* Valid only if `0 ≤ y < index_memory.size`
* Otherwise: `IndexError`, if `y < 0` → `SyntaxError`

---

### 5.4 Control Flow: Jumps

#### `^y`

* Push `pc + 1` onto the data stack
* Set `pc = y`
* If `pc ≥ program length`: `IndexError`

#### `^s`

* Pop value from data stack
* Set `pc` to that value

---

### 5.5 Input / Output

#### `#`

* Output ASCII character from current register
* Valid range: `0–255`
* Otherwise: `ValueError`

#### `#!`

* Output numeric value of current register

#### `.`

* Read one character
* Store ASCII value in current register
* Echo the character

#### `:`

* Read signed integer
* Store in current register
* Echo numeric input

---

### 5.6 Stack Operations

#### `@`

* Push current register value onto data stack

#### `@R`

* Push runtime (milliseconds) onto data stack

#### `_`

* Pop from stack and **add** to current register
* If stack empty: `StackError`

---

### 5.7 Conditionals

#### `{a op b}( … )`

* `a`, `b` are either:

  * register indices, or
  * `i` meaning current pointer
* `op ∈ { =, <, > }`
* If condition true:

  * execution continues into `()` block
* If false:

  * VM skips forward to matching `)`

Errors:

* Invalid register index → `IndexError`
* Invalid operator → `SyntaxError`

---

### 5.8 Loops

#### `[`

* If current register is zero:

  * Skip to matching `]`
* Otherwise:

  * Push current `pc` onto loop stack

#### `]`

* If loop stack empty: `SyntaxError`
* If current register ≠ 0:

  * Jump back to last loop start
* Else:

  * Pop loop stack

#### `~`

* Valid only inside a loop
* Acts as `continue`, stops the loop if `memory[ptr] = 0`
* Outside loop: `SyntaxError`

---

### 5.9 Debugging

#### `d`

* Prints:

  * Memory
  * Pointer
  * Stack
  * Index memory
  * Loop stack
* No effect if `quiet` mode enabled

---

### 5.10 Termination

#### `;`

* Stops execution immediately
* Program state is printed unless `quiet`

---

### 5.11 Invalid Character

* → `SyntaxError`

---

## 6. Error Model

### 6.1 Errors Are Fatal

* No error is recoverable.
* Execution halts immediately.

### 6.2 Error Categories

* `IndexError`
* `StackError`
* `SyntaxError`
* `InputError`
* `ValueError`

### 6.3 Error Reporting

Each error reports:

* Instruction index
* Offending command
* Relevant parameters
* Human-readable hint (implementation detail)

---

## 7. Runtime

* Runtime is measured in seconds internally (`double`)
* Exposed to programs in milliseconds

---

## Author

Dooblix © 2026
