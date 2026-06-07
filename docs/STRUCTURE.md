# Structure

This document describes how the repository is laid out, the on-disk file
formats, and the in-memory data structures.

## Repository layout

```
Simple_VM/
├── include/
│   ├── vm.h          # core types (memory, instruction, vm) + the VM API
│   └── execute.h     # the opcode enum + the execute()/get_register() API
├── src/
│   ├── main.c        # entry point: argument dispatch (create vs. run)
│   ├── vm.c          # loaders + the fetch/decode/execute/writeback pipeline
│   └── execute.c     # the ALU and the per-opcode work
├── docs/
│   ├── STRUCTURE.md  # this file
│   └── DESIGN.md     # the design and philosophy
├── README.md
└── LICENSE           # MIT
```

## The two address spaces

The machine has two completely separate spaces, each loaded from its own file:

- **Code space** — an array of `instruction` values, loaded by `read_code`.
  Indexed by the instruction pointer (`ip`).
- **Data space** — a byte buffer (`memory.value`), loaded by `read_memory`.
  Indexed by a byte address held in a register.

Neither one can be reached as if it were the other. See
[DESIGN.md](DESIGN.md) for why.

## Data structures (`include/vm.h`)

### `memory` — the data space

```c
typedef struct memory {
    struct {
        uint32_t magic_n;    // 0xFFD8DEAD
        uint16_t version;    // format version (currently 1)
        uint16_t data_size;  // number of data bytes that follow
    } meta;
    uint8_t value[];         // flexible array: the actual data bytes
} memory;
```

The struct is allocated as `sizeof(memory) + data_size` so the data sits
directly after the header in one block.

### `instruction` — one 16-bit word

```c
typedef struct instruction {
    uint16_t opcode  : 3;   // operation (see execute.h enum)
    uint16_t r0      : 2;   // first operand register
    uint16_t r1      : 2;   // second operand register
    uint16_t r2      : 2;   // destination register
    uint16_t is_data : 1;   // 1 -> the address operand refers to data space
} instruction;
```

10 of the 16 bits are used; the remaining 6 are reserved for later growth
(immediates, more opcodes, addressing modes — none defined yet).

### `vm` — the machine state

```c
typedef struct vm {
    instruction ir;     // instruction register (the word being run)
    uint16_t ip;        // instruction pointer (index into code space)
    uint16_t r0, r1, r2, r3;
    memory  *mem;       // the data space (set once by run_vm)
} vm;
```

## On-disk formats

### Data file (`.tda`)

```
offset  size  field
0       4     magic_n   (VM_MAGIC = 0xFFD8DEAD)
4       2     version
6       2     data_size (N)
8       N     data bytes (zero-filled on creation)
```

Created by `creat_memory(path, size)` and read back by `read_memory(path)`.

`read_memory` validates the header before trusting the file: it reads the
`meta` block and checks that `magic_n == VM_MAGIC`. If the magic does not match
(the file is not a Simple_VM data file), or any read/allocation fails, it
returns `NULL` instead of loading a bogus space — and `main` reports the error
and stops. The magic value is defined once as `VM_MAGIC` in
[include/vm.h](../include/vm.h) and shared by both the writer and the reader.

### Code file

A flat sequence of `instruction` words (2 bytes each), with no header.
`read_code` divides the file size by `sizeof(instruction)` to get the
instruction count.

## Instruction set (`include/execute.h`)

| opcode | value | operands used      | effect                                  |
|--------|-------|--------------------|-----------------------------------------|
| `ADD`  | 0     | r0, r1 -> r2       | `r2 = r0 + r1`                          |
| `SUB`  | 1     | r0, r1 -> r2       | `r2 = r0 - r1`                          |
| `DW`   | 2     | r0 (addr) -> r2    | load: `r2 = data[r0]` (needs `is_data`) |
| `SW`   | 3     | r0 (addr), r1 (val)| store: `data[r0] = r1` (needs `is_data`)|
| `AND`  | 4     | r0, r1 -> r2       | `r2 = r0 & r1`                          |
| `OR`   | 5     | r0, r1 -> r2       | `r2 = r0 \| r1`                         |
| `NOT`  | 6     | r0 -> r2           | `r2 = ~r0`                              |
| `JUMP` | 7     | r0                 | `ip = r0`                               |

Here `r0`/`r1`/`r2` name the *register selected by* the matching bit-field.
Data words are 16-bit, little-endian, and byte-addressed: a word at address
`a` occupies `value[a]` (low byte) and `value[a+1]` (high byte).
