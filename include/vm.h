#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stddef.h>

typedef struct memory
{
    struct {
        uint32_t magic_n;
        uint16_t version;
        uint16_t data_size;
    }meta;
    uint8_t value[];
}memory;

typedef struct instruction
{
    uint16_t opcode:3;
    uint16_t r0:2;
    uint16_t r1:2;
    uint16_t r2:2;
    uint16_t is_data:1;
}instruction;

typedef struct vm
{
    instruction ir;
    uint16_t ip;
    uint16_t r0;
    uint16_t r1;
    uint16_t r2;
    uint16_t r3;
    memory *mem;
}vm;


int creat_memory(const char *path, uint16_t data_size);
memory *read_memory(const char *path);
instruction *read_code(const char *path, size_t *code_count);
int run_vm(memory *mem, instruction *code, size_t code_count);
int fetch(vm*, instruction *code);
int decode(instruction ir);
int writeback(vm *v);

#endif
