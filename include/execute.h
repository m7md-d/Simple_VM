#ifndef EXECUTE_H
#define EXECUTE_H

#include "vm.h"

typedef enum InstructionOpcode {
    OPCODE_ADD,
    OPCODE_SUB,
    OPCODE_DW,
    OPCODE_SW,
    OPCODE_AND,
    OPCODE_OR,
    OPCODE_NOT,
    OPCODE_JUMP,
    OPCODE_COUNT
}InstructionOpcode;

uint16_t *get_register(vm *v, uint16_t idx);
int execute(vm *v);

#endif
