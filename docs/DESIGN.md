# Design & Philosophy

Simple_VM is small on purpose. The point is not to be a fast or feature-rich
interpreter — it is to make one architectural decision and follow it honestly:
**code and data are two separate worlds, and they can never read each other.**

## The core idea: a Harvard machine

Most toy VMs put code and data in one flat array. That is convenient, but it
also means an instruction can read itself, overwrite the next instruction, or
jump into the middle of a number. Simple_VM refuses that:

- The **code** comes from one file and lives in its own array (`instruction *`).
- The **data** comes from another file and lives in its own buffer
  (`memory.value`).
- They are never the same allocation, and there is no instruction that turns a
  code address into a data address or vice versa.

This separation is the security philosophy of the project. Code cannot be
treated as data, and data cannot be executed — not because of a runtime check
that might be bypassed, but because the two simply do not live in the same
space.

## The `is_data` flag

Every instruction carries one bit, `is_data`. It answers a single question:
**when this instruction names an address, which world does that address belong
to?**

- `is_data == 0` (the default): the address is a *code* address.
- `is_data == 1`: the address is a *data* address.

The memory instructions enforce this:

- `DW` (load) reads a word from the data space. It only proceeds when
  `is_data == 1`. A load with `is_data == 0` would mean "read code as data",
  which is exactly what the model forbids, so it is rejected.
- `SW` (store) writes a word to the data space, and likewise requires
  `is_data == 1`. A store can never land in the code space.

So the flag is not decoration — it is the gate that keeps the two spaces
isolated. The default value (`0`) means "this is about code", which is the safe
default: nothing touches data unless it asks to.

## Trusting the data file

A data file carries a magic number, `VM_MAGIC` (`0xFFD8DEAD`), written into its
header by `creat_memory`. `read_memory` does not blindly trust whatever it is
handed: it reads the header and checks the magic first. If the value is wrong —
the file is not a Simple_VM data file, or it is truncated/corrupt — the loader
refuses it and returns `NULL` rather than building a space from garbage. The
magic lives in one place (`VM_MAGIC` in `include/vm.h`) so the writer and the
reader can never drift apart.

## The execution cycle

The whole machine is one loop in `run_vm`, and it runs the same four stages for
every instruction. Keeping these as four distinct calls is deliberate — it
mirrors how real hardware separates the phases, and it gives each phase exactly
one job.

```
while (ip < code_count):
    fetch      ->  copy code[ip] into the instruction register, ip++
    decode     ->  sanity-check the opcode
    execute    ->  do the ALU work / loads / jumps
    writeback  ->  commit a store to the data space
```

Why the work is split between `execute` and `writeback`:

- `execute` handles everything that ends up in a **register** or in the control
  flow: the arithmetic and logic ops, the `DW` load, and `JUMP`.
- `writeback` handles the one thing that reaches **out into the data space**:
  the `SW` store. Putting it in its own stage means there is a single, obvious
  place where the machine is allowed to mutate data memory — and that is the
  same place the `is_data` isolation rule is enforced.

The loop ends naturally when `ip` walks off the end of the code. There is no
`HALT` opcode yet; that is a deliberate gap, not an oversight.

## Argument handling: smart, not flagged

`main.c` has no option parser and no flags. It splits its work by *meaning*:

- The first argument is **always** the data file.
- The second argument is read for what it is: a run of digits means "create a
  data file this many bytes long"; anything else is treated as a path to the
  code file to execute.

This keeps the tool's surface tiny and matches the rest of the project — do one
thing, decide it from the shape of the input, and don't add machinery that
isn't needed yet.

## Deliberate gaps

These are not missing features so much as decisions left open:

- **No immediates.** Registers start at zero and there is no load-constant
  instruction yet; the 6 unused instruction bits are the obvious home for one.
- **No `HALT`.** Programs end by running off the end of the code.
- **No conditional or relative jumps.** `JUMP` is an absolute jump to a register
  value.
- **Stores are not persisted.** Data is mutated in memory while running; writing
  it back to the `.tda` file is left for later.

Each of these has a clean place to slot into the current design when the time
comes.
