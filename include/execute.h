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
    OPCODE_JUMP
}InstructionOpcode;

const OpcodeAction instructions[32] = {
    {OPCODE_AND, opcode_add},
    {OPCODE_OR, opcode_sub},
    {OPCODE_NOT, opcode_dw},
    {OPCODE_AND, opcode_sw},
    {OPCODE_OR, opcode_and},
    {OPCODE_NOT, opcode_not},
    {OPCODE_AND, opcode_jump},

}




#endif