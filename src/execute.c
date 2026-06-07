#include "execute.h"
#include <stdio.h>
#include <stdint.h>

uint16_t *get_register(vm *v, uint16_t idx) {
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

    uint16_t *r_src1 = get_register(v, src1_idx);
    uint16_t *r_src2 = get_register(v, src2_idx);
    uint16_t *r_dest = get_register(v, dest_idx);

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
            if (!v->ir.is_data || !v->mem) return -1;
            {
                uint16_t addr = *r_src1;
                if ((size_t)addr + 1 >= v->mem->meta.data_size) return -1;
                *r_dest = (uint16_t)v->mem->value[addr]
                        | ((uint16_t)v->mem->value[addr + 1] << 8);
            }
            break;
        case OPCODE_SW:
            /* the store is in the writeback stage. */
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
