#include <stdbool.h>
#include <stdint.h>

bool opcode_add (vm *vm, InstructionOpcode op, uint16_t register_type,
                 uint16_t r0, uint16_t r1, uint16_t *out)
{
 
    (void)vm; (void)op; (void)register_type;
    if (!out) return false;
    *out = r0 + r1;
    return true;

}

bool opcode_sub (vm *vm, InstructionOpcode op, uint16_t register_type,
                 uint16_t r0, uint16_t r1, uint16_t *out)
{
 
    (void)vm; (void)op; (void)register_type;
    if (!out) return false;
    *out = r0 - r1;
    return true;

}
bool opcode_dw (vm *vm, InstructionOpcode op, uint16_t register_type,
                 uint16_t r0, uint16_t r1, uint16_t *out)
{}
bool opcode_sw (vm *vm, InstructionOpcode op, uint16_t register_type,
                 uint16_t r0, uint16_t r1, uint16_t *out)
{}
bool opcode_or (vm *vm, InstructionOpcode op, uint16_t register_type,
                 uint16_t r0, uint16_t r1, uint16_t *out)
{
 
    (void)vm; (void)op; (void)register_type;
    if (!out) return false;
    *out = r0 | r1;
    return true;

}
bool opcode_not (vm *vm, InstructionOpcode op, uint16_t register_type,
                 uint16_t r0, uint16_t r1, uint16_t *out)
{
 
    (void)vm; (void)op; (void)register_type;
    if (!out) return false;
    *out = ~r0;
    return true;

}
bool opcode_jump (vm *vm, InstructionOpcode op, uint16_t register_type,
                 uint16_t r0, uint16_t r1, uint16_t *out)
{}