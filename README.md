# Simple_VM

A tiny 16-bit virtual machine written in C, built around one idea: **code and
data never share an address space.** The VM follows a Harvard model — the
program lives in one file, the data lives in another, and an instruction can
only touch the data space when it explicitly says so.

## What it does

The VM runs a classic four-stage cycle for every instruction:

```
fetch  ->  decode  ->  execute  ->  writeback
```

All four stages are driven from a single loop in `run_vm` (see
[src/vm.c](src/vm.c)). It keeps fetching instructions until the instruction
pointer runs past the end of the code.

- **4 registers**: `r0`–`r3`, each 16-bit.
- **8 opcodes**: `ADD`, `SUB`, `DW` (load), `SW` (store), `AND`, `OR`, `NOT`,
  `JUMP`.
- **Two separate files**: a data file and a code file. They are loaded into two
  separate spaces that cannot reach into each other.

## Building

```sh
gcc -Wall -Wextra -Iinclude src/*.c -o svm
```

## Usage

The first argument is **always** the data file. The second argument decides
what happens:

```sh
# 1. second arg is a number  -> create a zeroed data file of that many bytes
./svm program.tda 64

# 2. second arg is a path    -> that file is the code; load both and run
./svm program.tda program.code
```

There are no command-line flags — the program tells the two modes apart by
looking at whether the second argument is a number or a path.

## How an instruction is shaped

Each instruction is 16 bits:

| field     | bits | meaning                                   |
|-----------|------|-------------------------------------------|
| `opcode`  | 3    | which operation                           |
| `r0`      | 2    | first register operand                    |
| `r1`      | 2    | second register operand                   |
| `r2`      | 2    | destination register                      |
| `is_data` | 1    | when set, the address operand is **data** |

The `is_data` flag is the heart of the design: a load (`DW`) or store (`SW`)
is only allowed to touch the data space when `is_data == 1`. This is what keeps
code and data apart — it is a safety property, not just a convenience.

## Docs

- [docs/STRUCTURE.md](docs/STRUCTURE.md) — repository layout, file formats, and
  the data structures.
- [docs/DESIGN.md](docs/DESIGN.md) — the design, how it runs, and the
  philosophy behind the code/data split.

## License

[MIT](LICENSE).
