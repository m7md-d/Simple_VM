#include "EXECUTE_H"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


const OpcodeAction instructions[OPCODE_COUNT] = {
    [OPCODE_ADD] = {OPCODE_ADD, opcode_add},
    [OPCODE_SUB] = {OPCODE_SUB, opcode_sub},
    [OPCODE_DW]  = {OPCODE_DW,  opcode_dw},
    [OPCODE_SW]  = {OPCODE_SW,  opcode_sw},
    [OPCODE_AND] = {OPCODE_AND, opcode_and},
    [OPCODE_OR]  = {OPCODE_OR,  opcode_or},
    [OPCODE_NOT] = {OPCODE_NOT, opcode_not},
    [OPCODE_JUMP]= {OPCODE_JUMP,opcode_jump}
};

static void set_register_value(vm *v, uint16_t idx, uint16_t val) {
    switch (idx) {
        case 0: v->r0 = val; break;
        case 1: v->r1 = val; break;
        case 2: v->r2 = val; break;
        case 3: v->r3 = val; break;
    }
}

static unit16* get_register(vm *v, uint16_t idx) {
    switch (idx) {
        case 0: return &v->r0;
        case 1: return &v->r1;
        case 2: return &v->r2;
        case 3: return &v->r3;
        default: return NULL;
    }
}

int execute(vm *v) {
    if (!v) return -1;

    uint16_t opcode = v->ir.opcode;

    uint16_t src1_idx = v->ir.r0;
    uint16_t src2_idx = v->ir.r1;
    uint16_t dest_idx = v->ir.r2;

    unit16 *r_src1 = get_register(v, src1_idx);
    unit16 *r_src2 = get_register(v, src2_idx);
    unit16 *r_dest = get_register(v, dest_idx);

    if (!r_src1 || !r_dest) return -1;

    switch (opcode) {
        case OPCODE_ADD:
            if (!r_src2) return -1;
            *r_dest = *r_src1 + *r_src2;
            break;
        case OPCODE_SUB:
            if (!r_src2) return -1;
            *r_dest = *r_src1 - *r_src2;
            break;
        case OPCODE_DW:
        fprintf(stderr, "DW not implemented\n");
            break;
        case OPCODE_SW:
            fprintf(stderr, "SW not implemented\n");
            break;
        case OPCODE_AND:
            if (!r_src2) return -1;
            *r_dest = *r_src1 & *r_src2;
            break;
        case OPCODE_OR:
            if (!r_src2) return -1;
            *r_dest = *r_src1 | *r_src2;
            break;
        case OPCODE_NOT:
            *r_dest = ~(*r_src1);
            break;
        case OPCODE_JUMP:
            v->ip = *r_src1;
            break;
        default:
            fprintf(stderr, "Unknown opcode: %d\n", opcode);
            return -1;
    }
    return 0;
}

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